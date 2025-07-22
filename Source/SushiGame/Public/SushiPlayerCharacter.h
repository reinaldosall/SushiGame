#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SushiPlayerCharacter.generated.h"

class ACookwareActor;
class AIngredientActor;
class ATableActor;

UCLASS()
class SUSHIGAME_API ASushiPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASushiPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Rate);
	void LookUp(float Rate);

	// Interaction
	void Interact();
	void DeliverDish();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteractWith(ACookwareActor* Cookware, AIngredientActor* Ingredient);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDeliverDish(FName RecipeName, ATableActor* Table);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

public:
	UPROPERTY(Replicated)
	FName HeldRecipe;

	UPROPERTY(Replicated)
	int32 RecipeProgress;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickupIngredient(AIngredientActor* Ingredient);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};