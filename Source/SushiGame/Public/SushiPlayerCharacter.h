#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SushiPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ATableActor;

UCLASS()
class SUSHIGAME_API ASushiPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASushiPlayerCharacter();

protected:
	virtual void BeginPlay() override;

	// Variáveis de gameplay
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input handlers
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Rate);
	void LookUp(float Rate);

	void Interact();
	void DeliverDish();	

	// Estado da receita atual
	UPROPERTY(BlueprintReadWrite, Replicated)
	FName HeldRecipe;

	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 RecipeProgress;
	
	// Chamado no servidor para entregar o prato
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDeliverDish(FName RecipeName, ATableActor* Table);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteract();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteractWith(ACookwareActor* Cookware, AIngredientActor* Ingredient);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
