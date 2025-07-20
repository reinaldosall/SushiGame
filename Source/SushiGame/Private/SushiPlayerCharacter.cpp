// Fill out your copyright notice in the Description page of Project Settings.


#include "SushiPlayerCharacter.h"
#include "CookwareActor.h"
#include "IngredientActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASushiPlayerCharacter::ASushiPlayerCharacter()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;

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

}

// Called when the game starts or when spawned
void ASushiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

	PlayerInputComponent->BindAxis("MoveForward", this, &ASushiPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASushiPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ASushiPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASushiPlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASushiPlayerCharacter::Interact);
}

void ASushiPlayerCharacter::Interact()
{
	FVector Start = GetActorLocation() + FVector(0, 0, 50);
	FVector End = Start + GetActorForwardVector() * 500.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.0f, 0, 2.0f);

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *Hit.GetActor()->GetName());

		if (AIngredientActor* Ingredient = Cast<AIngredientActor>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Directly hit an IngredientActor!"));
			Ingredient->OnInteract();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing hit."));
	}
}

void ASushiPlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASushiPlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
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



