#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IngredientActor.generated.h"

UENUM(BlueprintType)
enum class EIngredientState : uint8
{
	Raw        UMETA(DisplayName = "Raw"),
	Sliced     UMETA(DisplayName = "Sliced"),
	Rolled     UMETA(DisplayName = "Rolled"),
	Cooked     UMETA(DisplayName = "Cooked"),
	Finished   UMETA(DisplayName = "Finished")
};

UCLASS()
class SUSHIGAME_API AIngredientActor : public AActor
{
	GENERATED_BODY()

public:	
	AIngredientActor();

protected:
	virtual void BeginPlay() override;

	// State of the ingredient (e.g., Raw, Sliced, etc.)
	UPROPERTY(ReplicatedUsing = OnRep_IngredientState, VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EIngredientState IngredientState;

	UFUNCTION()
	void OnRep_IngredientState();

	// Static mesh for visual representation
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	void UpdateVisualForState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Called when the player interacts with the ingredient (via CookwareActor, etc)
	void OnInteract();

	UFUNCTION(BlueprintCallable)
	EIngredientState GetIngredientState() const { return IngredientState; }

	UFUNCTION(BlueprintCallable)
	void SetIngredientState(EIngredientState NewState);
};
