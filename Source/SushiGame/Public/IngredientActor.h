#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "IngredientActor.generated.h"

UENUM(BlueprintType)
enum class EIngredientState : uint8
{
	Raw,
	Sliced,
	Rolled,
	Finished
};

UCLASS()
class SUSHIGAME_API AIngredientActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AIngredientActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* IngredientLabel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ingredient")
	FName IngredientType; // Ex: SushiRoll, Peixe, Carne etc

	UPROPERTY(ReplicatedUsing = OnRep_IngredientState)
	EIngredientState IngredientState;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetIngredientState(EIngredientState NewState);
	EIngredientState GetIngredientState() const { return IngredientState; }

	UFUNCTION()
	void OnRep_IngredientState();

	UFUNCTION(BlueprintCallable)
	void OnInteract();

protected:
	virtual void BeginPlay() override;
	void UpdateVisualForState();
};
