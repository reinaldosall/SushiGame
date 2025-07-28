#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CookwareActor.generated.h"

class UWidgetComponent;
class AIngredientActor;
class ASushiPlayerCharacter;

UCLASS()
class SUSHIGAME_API ACookwareActor : public AActor
{
	GENERATED_BODY()

public:
	ACookwareActor();
	virtual void Tick(float DeltaTime) override;

	// Interação com o jogador
	void OnInteract(ASushiPlayerCharacter* Player);

	FTimerHandle CookingTimerHandle;
	
	UPROPERTY(ReplicatedUsing = OnRep_CookingElapsedTime)
	float CookingElapsedTime = 0.f;
	float CookingDuration = 5.f;

	
 
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* ProgressWidget;

	UPROPERTY(ReplicatedUsing = OnRep_RecipeProgress)
	int32 SharedRecipeProgress = 0;

	UPROPERTY(ReplicatedUsing = OnRep_IsCooking)
	bool bIsCooking = false;

	UPROPERTY()
	ASushiPlayerCharacter* LockedPlayer;

	void StartCooking(ASushiPlayerCharacter* Player);
	void FinishCooking();

	UFUNCTION()
	void OnRep_RecipeProgress();

	UFUNCTION()
	void OnRep_IsCooking();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateProgress();

	UFUNCTION()
	void OnRep_CookingElapsedTime();

	void UpdateProgressUI();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
