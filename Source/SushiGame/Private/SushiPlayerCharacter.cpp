#include "SushiPlayerCharacter.h"
#include "CookwareActor.h"
#include "IngredientActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "OrderManager.h"
#include "TableActor.h"
#include "SushiPlayerState.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
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

			if (!HeldRecipe.IsNone())
			{
				for (TActorIterator<AIngredientActor> It(GetWorld()); It; ++It)
				{
					AIngredientActor* Ingredient = *It;
					if (Ingredient && Ingredient->IngredientType == HeldRecipe)
					{
						ServerInteractWith(Cookware, Ingredient);
						return;
					}
				}

				UE_LOG(LogTemp, Warning, TEXT("No ingredient found matching held recipe: %s"), *HeldRecipe.ToString());
			}
		}
	}
}


void ASushiPlayerCharacter::ServerInteractWith_Implementation(ACookwareActor* Cookware, AIngredientActor* Ingredient)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerInteractWith called"));
	UE_LOG(LogTemp, Warning, TEXT("ServerInteractWith called with %s and %s"),
	*GetNameSafe(Cookware), *GetNameSafe(Ingredient));
	if (Cookware && Ingredient)
	{
		Cookware->OnInteract(Ingredient);
	}
}

bool ASushiPlayerCharacter::ServerInteractWith_Validate(ACookwareActor* Cookware, AIngredientActor* Ingredient)
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
	if (RecipeProgress < 3)
	{
		if (ASushiPlayerState* PS = Cast<ASushiPlayerState>(GetPlayerState()))
		{
			PS->AddScore(-50);
		}
		return;
	}

	for (TActorIterator<AOrderManager> It(GetWorld()); It; ++It)
	{
		AOrderManager* Manager = *It;
		if (!Manager) continue;

		UE_LOG(LogTemp, Warning, TEXT("Delivering %s to table %s"), *RecipeName.ToString(), *Table->GetName());

		EDeliveryResult Result = Manager->TryCompleteOrder(RecipeName, Table);
		if (ASushiPlayerState* PS = Cast<ASushiPlayerState>(GetPlayerState()))
		{
			switch (Result)
			{
			case EDeliveryResult::Success:
				PS->AddScore(100);
				HeldRecipe = NAME_None;
				RecipeProgress = 0;
				break;
			case EDeliveryResult::WrongRecipe:
			case EDeliveryResult::WrongTable:
				PS->AddScore(-50);
				break;
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

	DOREPLIFETIME(ASushiPlayerCharacter, HeldRecipe);
	DOREPLIFETIME(ASushiPlayerCharacter, RecipeProgress);
}

void ASushiPlayerCharacter::ServerPickupIngredient_Implementation(AIngredientActor* Ingredient)
{
	if (Ingredient)
	{
		HeldRecipe = Ingredient->IngredientType;
		RecipeProgress = 0;

		UE_LOG(LogTemp, Warning, TEXT("Picked up ingredient: %s"), *HeldRecipe.ToString());
	}
}


bool ASushiPlayerCharacter::ServerPickupIngredient_Validate(AIngredientActor* Ingredient)
{
	return true;
}
