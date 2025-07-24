#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "OrderManager.generated.h"

class ATableActor;
class UOrderHUDWidget;

UENUM(BlueprintType)
enum class EDeliveryResult : uint8
{
	Success,
	WrongRecipe,
	WrongTable
};

USTRUCT(BlueprintType)
struct FOrder
{
	GENERATED_BODY()

	UPROPERTY()
	FName RecipeName;

	UPROPERTY()
	float TimeRemaining;

	UPROPERTY()
	ATableActor* TargetTable;

	UPROPERTY()
	bool bCompleted;
};

UCLASS()
class SUSHIGAME_API AOrderManager : public AActor
{
	GENERATED_BODY()

public:
	AOrderManager();
	
	UPROPERTY(VisibleAnywhere)
	UBillboardComponent* EditorIcon;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orders")
	TArray<FName> AvailableRecipes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orders")
	float MaxOrderTime = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orders")
	float OrderInterval = 5.0f;

	UPROPERTY()
	float TimeSinceLastOrder = 0.0f;

	UPROPERTY(ReplicatedUsing = OnRep_ActiveOrders)
	TArray<FOrder> ActiveOrders;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOrderHUDWidget> HUDWidgetClass;

	UPROPERTY()
	UOrderHUDWidget* HUDWidgetInstance;

	UFUNCTION()
	void OnRep_ActiveOrders();

	void UpdateHUD();

public:
	UFUNCTION()
	void GenerateOrder();

	UFUNCTION()
	EDeliveryResult TryCompleteOrder(FName RecipeName, ATableActor* Table);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
