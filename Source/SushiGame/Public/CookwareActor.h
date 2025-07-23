#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CookwareActor.generated.h"

class UStaticMeshComponent;
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

	void OnInteract(ASushiPlayerCharacter* Player);

protected:
	virtual void BeginPlay() override;

	// Mesh e Widget
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CookwareMesh;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* ProgressWidget;

	// Cooking logic
	UPROPERTY()
	ASushiPlayerCharacter* CookingPlayer;

	UPROPERTY()
	bool bIsCooking;

	UPROPERTY()
	float CookingProgress;

	UPROPERTY(EditDefaultsOnly)
	float CookingDuration = 5.0f;

	FTimerHandle ResetTimerHandle;

	// UI helpers
	void UpdateProgressWidget(const FString& StatusText, float Percent);
	void ResetProgressWidget();
};
