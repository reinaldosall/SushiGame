#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

public:	
	virtual void Tick(float DeltaTime) override;

	// Called when the player interacts with this actor
	void OnInteract(AIngredientActor* Ingredient);

protected:

	// Simulates time needed to process an ingredient
	UPROPERTY(EditDefaultsOnly, Category = "Cooking")
	float ProcessingTime = 2.5f;

	// Current ingredient being processed
	UPROPERTY(Replicated)
	AIngredientActor* CurrentIngredient;

	// Whether we are currently processing
	UPROPERTY(Replicated)
	bool bIsProcessing;

	// Timer handle
	FTimerHandle ProcessingTimerHandle;

	UFUNCTION()
	void OnProcessingComplete();

	void StartProcessing(AIngredientActor* Ingredient);

	// Replication setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};