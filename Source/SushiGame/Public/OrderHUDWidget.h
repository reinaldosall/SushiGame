#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrderHUDWidget.generated.h"

USTRUCT(BlueprintType)
struct FOrderDisplayData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName RecipeName;

	UPROPERTY(BlueprintReadWrite)
	float TimeRemaining;

	UPROPERTY(BlueprintReadWrite)
	int32 TableID;
};

UCLASS()
class SUSHIGAME_API UOrderHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Called by OrderManager to update the list
	UFUNCTION(BlueprintImplementableEvent, Category = "Orders")
	void UpdateOrdersUI(const TArray<FOrderDisplayData>& Orders);
};