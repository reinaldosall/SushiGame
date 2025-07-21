#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrderManager.generated.h"

class ATableActor;
class UUserWidget;

USTRUCT(BlueprintType)
struct FOrder
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RecipeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeRemaining;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATableActor* TargetTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCompleted = false;
};

UCLASS()
class SUSHIGAME_API AOrderManager : public AActor
{
	GENERATED_BODY()

public:	
	AOrderManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Spawns a new random order
	void GenerateOrder();

	// Called when player delivers a dish
	bool TryCompleteOrder(FName RecipeName, ATableActor* Table);

	UPROPERTY(EditAnywhere, Category = "Orders")
	TArray<FName> AvailableRecipes;

	UPROPERTY(EditAnywhere, Category = "Orders")
	float OrderInterval = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Orders")
	float MaxOrderTime = 30.0f;

	UPROPERTY(BlueprintReadOnly)
	TArray<FOrder> ActiveOrders;

private:
	float TimeSinceLastOrder = 0.0f;
};