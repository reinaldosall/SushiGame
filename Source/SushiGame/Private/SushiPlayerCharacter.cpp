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
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bReplicates = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attach boom to root
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Attach camera to boom
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	// Allow character rotation with input
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	UE_LOG(LogTemp, Warning, TEXT("Pawn possessed at BeginPlay: %s"), *GetName());

}

// Called when the game starts or when spawned
void ASushiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AController* MyController = GetController())
	{
		UE_LOG(LogTemp, Warning, TEXT("SushiPlayerCharacter possessed by: %s"), *MyController->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SushiPlayerCharacter was not possessed at BeginPlay"));
	}
}

// Called every frame
void ASushiPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input

void ASushiPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent called"));

	PlayerInputComponent->BindAxis("MoveForward", this, &ASushiPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASushiPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ASushiPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASushiPlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASushiPlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Deliver", IE_Pressed, this, &ASushiPlayerCharacter::DeliverDish);

}

void ASushiPlayerCharacter::Interact()
{
	FVector Start = GetActorLocation() + FVector(0, 0, 50);
	FVector End = Start + GetActorForwardVector() * 300.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult CookwareHit;
	if (GetWorld()->LineTraceSingleByChannel(CookwareHit, Start, End, ECC_Visibility, Params))
	{
		if (ACookwareActor* Cookware = Cast<ACookwareActor>(CookwareHit.GetActor()))
		{
			// Segundo traço: procurar ingrediente na frente
			FVector CheckStart = GetActorLocation() + FVector(0, 0, 50);
			FVector CheckEnd = CheckStart + GetActorForwardVector() * 150.f;

			FHitResult IngredientHit;
			if (GetWorld()->LineTraceSingleByChannel(IngredientHit, CheckStart, CheckEnd, ECC_Visibility, Params))
			{
				if (AIngredientActor* Ingredient = Cast<AIngredientActor>(IngredientHit.GetActor()))
				{
					ServerInteractWith(Cookware, Ingredient);
				}
			}
		}
	}
}

void ASushiPlayerCharacter::DeliverDish()
{
	FVector Start = GetActorLocation() + FVector(0, 0, 50);
	FVector End = Start + GetActorForwardVector() * 300.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		if (ATableActor* Table = Cast<ATableActor>(Hit.GetActor()))
		{
			if (!HeldRecipe.IsNone())
			{
				ServerDeliverDish(HeldRecipe, Table);
			}
		}
	}
}

void ASushiPlayerCharacter::ServerInteract_Implementation()
{
	// Primeira linha: cookware
	FVector Start = GetActorLocation() + FVector(0, 0, 50);
	FVector End = Start + GetActorForwardVector() * 300.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult CookwareHit;
	if (GetWorld()->LineTraceSingleByChannel(CookwareHit, Start, End, ECC_Visibility, Params))
	{
		if (ACookwareActor* Cookware = Cast<ACookwareActor>(CookwareHit.GetActor()))
		{
			// Segunda linha: ingrediente
			FVector CheckStart = GetActorLocation() + FVector(0, 0, 50);
			FVector CheckEnd = CheckStart + GetActorForwardVector() * 150.f;

			FHitResult IngredientHit;
			if (GetWorld()->LineTraceSingleByChannel(IngredientHit, CheckStart, CheckEnd, ECC_Visibility, Params))
			{
				if (AIngredientActor* Ingredient = Cast<AIngredientActor>(IngredientHit.GetActor()))
				{
					Cookware->OnInteract(Ingredient);
				}
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

void ASushiPlayerCharacter::Turn(float Rate)
{
	AddControllerYawInput(Rate);
}

void ASushiPlayerCharacter::LookUp(float Rate)
{
	AddControllerPitchInput(Rate);
}
void ASushiPlayerCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void ASushiPlayerCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

bool ASushiPlayerCharacter::ServerInteract_Validate()
{
	return true;
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

void ASushiPlayerCharacter::ServerInteractWith_Implementation(ACookwareActor* Cookware, AIngredientActor* Ingredient)
{
	if (Cookware && Ingredient)
	{
		Cookware->OnInteract(Ingredient);
	}
}

bool ASushiPlayerCharacter::ServerInteractWith_Validate(ACookwareActor* Cookware, AIngredientActor* Ingredient)
{
	return true;
}