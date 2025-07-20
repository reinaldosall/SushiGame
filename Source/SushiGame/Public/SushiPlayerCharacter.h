#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SushiPlayerCharacter.generated.h"

UCLASS()
class SUSHIGAME_API ASushiPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASushiPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Interact();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Rate);
	void LookUp(float Rate);
};