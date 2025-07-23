#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "CookwareActor.generated.h"

class AIngredientActor;

UCLASS()
class SUSHIGAME_API ACookwareActor : public AActor
{
	GENERATED_BODY()

public:
	ACookwareActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* CookwareMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* ProgressWidget;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentIngredient)
	AIngredientActor* CurrentIngredient;

	UPROPERTY(ReplicatedUsing = OnRep_IsCooking)
	bool bIsCooking;

	UPROPERTY(ReplicatedUsing = OnRep_IsDone)
	bool bIsDone;

	UPROPERTY()
	APlayerController* LockedPlayer;

	FTimerHandle CookingTimerHandle;

public:
	virtual void Tick(float DeltaTime) override;

	void OnInteract(AIngredientActor* Ingredient);
	void OnProcessingComplete();

	UFUNCTION()
	void OnCookingFinished();

	void UpdateProgressWidget();

	UFUNCTION()
	void OnRep_CurrentIngredient();

	UFUNCTION()
	void OnRep_IsCooking();

	UFUNCTION()
	void OnRep_IsDone();

	UPROPERTY()
	float CookingElapsedTime;

	UPROPERTY(EditDefaultsOnly, Category = "Cooking")
	float CookingDuration = 5.0f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
