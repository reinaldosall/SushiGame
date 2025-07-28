#include "SushiPlayerCharacter.h"
#include "CookwareActor.h"
#include "IngredientActor.h"
#include "PlayerStatusWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "OrderManager.h"
#include "TableActor.h"
#include "SushiPlayerState.h"
#include "EngineUtils.h"
#include "SushiPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SushiGameState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASushiPlayerCharacter::ASushiPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;
}

void ASushiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASushiPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASushiPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASushiPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASushiPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ASushiPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASushiPlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASushiPlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Deliver", IE_Pressed, this, &ASushiPlayerCharacter::DeliverDish);
}

void ASushiPlayerCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASushiPlayerCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASushiPlayerCharacter::Turn(float Rate)
{
	AddControllerYawInput(Rate);
}

void ASushiPlayerCharacter::LookUp(float Rate)
{
	AddControllerPitchInput(Rate);
}

void ASushiPlayerCharacter::Interact()
{
	FVector Start = GetActorLocation() + FVector(0, 0, 50);
	FVector End = Start + GetActorForwardVector() * 300.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 2.0f);

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		// Se acertar um ingrediente
		if (AIngredientActor* Ingredient = Cast<AIngredientActor>(Hit.GetActor()))
		{
			ServerPickupIngredient(Ingredient);
			return;
		}

		// Se acertar uma cookware
		if (ACookwareActor* Cookware = Cast<ACookwareActor>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Cookware hit: %s"), *Cookware->GetName());

			// if (!HeldRecipe.IsNone())
			// {
			// 	ServerInteractWith(Cookware);
			// }
			// Dentro do LineTrace de hit:
						
			ServerInteractWith(Cookware);

		}
	}
}


void ASushiPlayerCharacter::ServerInteractWith_Implementation(ACookwareActor* Cookware)
{
	if (!Cookware || HeldRecipe.IsNone()) return;

	UE_LOG(LogTemp, Warning, TEXT("ServerInteractWith called — Player: %s, Recipe: %s"),
		*GetName(), *HeldRecipe.ToString());
	if (Cookware)
	{
		Cookware->OnInteract(this);
	}
	//Cookware->OnInteract(this); // Passa a si mesmo
}

bool ASushiPlayerCharacter::ServerInteractWith_Validate(ACookwareActor* Cookware)
{
	return true;
}


void ASushiPlayerCharacter::DeliverDish()
{
	FVector Start = GetActorLocation() + FVector(0, 0, 50);
	FVector End = Start + GetActorForwardVector() * 300.f;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		UE_LOG(LogTemp, Warning, TEXT("Pressed F - trying to deliver dish"));
		if (ATableActor* Table = Cast<ATableActor>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Table: %s"), *Table->GetName());
			if (!HeldRecipe.IsNone())
			{
				ServerDeliverDish(HeldRecipe, Table);
			}
		}
	}
}

void ASushiPlayerCharacter::ServerDeliverDish_Implementation(FName RecipeName, ATableActor* Table)
{
	if (!Table) return;

	ASushiGameState* GS = GetWorld() ? GetWorld()->GetGameState<ASushiGameState>() : nullptr;
	if (!GS) return;

	if (RecipeProgress < 4)
	{
		GS->AddGlobalScore(-50);
		Table->SetFeedbackText("X");
		ClientUpdateDeliverFeedback("X", GS->GetGlobalScore());
		return;
	}

	for (TActorIterator<AOrderManager> It(GetWorld()); It; ++It)
	{
		AOrderManager* Manager = *It;
		if (!Manager) continue;

		UE_LOG(LogTemp, Warning, TEXT("Delivering %s to table %s"), *RecipeName.ToString(), *Table->GetName());

		EDeliveryResult Result = Manager->TryCompleteOrder(RecipeName, Table);
		switch (Result)
		{
		case EDeliveryResult::Success:
			GS->AddGlobalScore(100);
			Table->SetFeedbackText("CORRECT");
			ClientUpdateDeliverFeedback("CORRECT", GS->GetGlobalScore());
			ClearHeldRecipe();
			break;
		case EDeliveryResult::WrongRecipe:
		case EDeliveryResult::WrongTable:
			GS->AddGlobalScore(-50);
			Table->SetFeedbackText("INCORRECT");
			ClientUpdateDeliverFeedback("INCORRECT", GS->GetGlobalScore());
			break;
		}
	}
}

void ASushiPlayerCharacter::ClientUpdateDeliverFeedback_Implementation(const FString& ResultSymbol, int32 NewScore)
{
	if (!IsLocallyControlled()) return;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ASushiPlayerController* SPC = Cast<ASushiPlayerController>(PC))
		{
			if (SPC->PlayerStatusWidgetInstance)
			{
				SPC->PlayerStatusWidgetInstance->UpdateDeliveryStatus(ResultSymbol);
				SPC->PlayerStatusWidgetInstance->UpdateScore(FString::Printf(TEXT("%d"), NewScore));
			}
		}
	}
}

bool ASushiPlayerCharacter::ServerDeliverDish_Validate(FName RecipeName, ATableActor* Table)
{
	return true;
}

void ASushiPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ASushiPlayerCharacter, HeldRecipe);
	//DOREPLIFETIME(ASushiPlayerCharacter, RecipeProgress);
	//DOREPLIFETIME_CONDITION_NOTIFY(ASushiPlayerCharacter, HeldRecipe, COND_OwnerOnly, REPNOTIFY_Always);
	//DOREPLIFETIME_CONDITION_NOTIFY(ASushiPlayerCharacter, RecipeProgress, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ASushiPlayerCharacter, HeldRecipe, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ASushiPlayerCharacter, RecipeProgress, COND_None, REPNOTIFY_Always);



}

void ASushiPlayerCharacter::ServerPickupIngredient_Implementation(AIngredientActor* Ingredient)
{
	if (Ingredient)
	{
		RecipeProgress = 0; // primeiro zera o progresso
		SetHeldRecipe(Ingredient->IngredientType); // depois atualiza HUD com valor correto
		
		UE_LOG(LogTemp, Warning, TEXT("Picked up ingredient: %s"), *HeldRecipe.ToString());

		if (IsLocallyControlled())  
		{
			UpdatePlayerStatusUI(); // opcional aqui, SetHeldRecipe já chama
		}
	}
}

bool ASushiPlayerCharacter::ServerPickupIngredient_Validate(AIngredientActor* Ingredient)
{
	return true;
}

void ASushiPlayerCharacter::OnRep_HeldRecipe()
{
	UE_LOG(LogTemp, Warning, TEXT(">>> OnRep_HeldRecipe triggered"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("OnRep_HeldRecipe called!"));
	UpdatePlayerStatusUI();
}

void ASushiPlayerCharacter::OnRep_RecipeProgress()
{
	UE_LOG(LogTemp, Warning, TEXT(">>> OnRep_RecipeProgress triggered"));
	UpdatePlayerStatusUI();
}

void ASushiPlayerCharacter::UpdatePlayerStatusUI()
{
	if (!IsLocallyControlled()) return;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	ASushiPlayerController* SPC = Cast<ASushiPlayerController>(PC);
	if (!SPC || !SPC->PlayerStatusWidgetInstance) return;

	FString RecipeStr = HeldRecipe.IsNone() ? TEXT("None") : HeldRecipe.ToString();
	FString StepStr = TEXT("Unknown");

	switch (RecipeProgress)
	{
	case 0: StepStr = "None"; break;
	case 1: StepStr = "Sliced"; break;
	case 2: StepStr = "Rolled"; break;
	case 3: StepStr = "Cooking"; break;
	case 4: StepStr = "Done"; break; 
	default: StepStr = "Unknown"; break;
	}

	UE_LOG(LogTemp, Warning, TEXT(">>> UpdatePlayerStatusUI() called — Recipe: %s, Step: %s"),
		*RecipeStr, *StepStr);

	SPC->PlayerStatusWidgetInstance->UpdateStatus(RecipeStr, StepStr);
	UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerStatusUI → HeldRecipe: %s, Progress: %d"),
	*HeldRecipe.ToString(), RecipeProgress);
}

void ASushiPlayerCharacter::SetHeldRecipe(FName NewRecipe)
{
	HeldRecipe = NAME_None;  
	HeldRecipe = NewRecipe;

	ForceNetUpdate();

	if (IsLocallyControlled())
	{
		UpdatePlayerStatusUI();
	}
}

void ASushiPlayerCharacter::SetRecipeProgress(int32 NewProgress)
{
	//RecipeProgress = -1;
	RecipeProgress = NewProgress;
	if (IsLocallyControlled())
	{
		UpdatePlayerStatusUI();
	}
}

void ASushiPlayerCharacter::ClearHeldRecipe()
{
	HeldRecipe = NAME_None;
	RecipeProgress = 0;

	ForceNetUpdate();

	if (IsLocallyControlled())
	{
		UpdatePlayerStatusUI();
	}
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ASushiPlayerController* SPC = Cast<ASushiPlayerController>(PC))
		{
			if (SPC->PlayerStatusWidgetInstance)
			{
				SPC->PlayerStatusWidgetInstance->UpdateDeliveryStatus(TEXT(""));
			}
		}
	}
}