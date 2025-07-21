#include "OrderManager.h"

#include "OrderHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TableActor.h"

AOrderManager::AOrderManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOrderManager::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UOrderHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
}

void AOrderManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastOrder += DeltaTime;

	// Spawn new order periodically
	if (TimeSinceLastOrder >= OrderInterval)
	{
		GenerateOrder();
		TimeSinceLastOrder = 0.0f;
	}

	// Update timers
	for (FOrder& Order : ActiveOrders)
	{
		if (!Order.bCompleted)
		{
			Order.TimeRemaining -= DeltaTime;

			if (Order.TimeRemaining <= 0.0f)
			{
				UE_LOG(LogTemp, Warning, TEXT("Order expired: %s"), *Order.RecipeName.ToString());
				Order.bCompleted = true;
			}
		}
	}

	if (HUDWidgetInstance)
	{
		TArray<FOrderDisplayData> DisplayList;
		for (const FOrder& Order : ActiveOrders)
		{
			if (!Order.bCompleted)
			{
				FOrderDisplayData Data;
				Data.RecipeName = Order.RecipeName;
				Data.TimeRemaining = Order.TimeRemaining;
				Data.TableID = Order.TargetTable ? Order.TargetTable->TableID : -1;
				DisplayList.Add(Data);
			}
		}
		HUDWidgetInstance->UpdateOrdersUI(DisplayList);
	}
}

void AOrderManager::GenerateOrder()
{
	if (AvailableRecipes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No recipes available to generate order."));
		return;
	}

	// Find a random table in the world
	TArray<AActor*> FoundTables;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATableActor::StaticClass(), FoundTables);

	if (FoundTables.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No tables found for order assignment."));
		return;
	}

	ATableActor* TargetTable = Cast<ATableActor>(FoundTables[FMath::RandRange(0, FoundTables.Num() - 1)]);
	FName ChosenRecipe = AvailableRecipes[FMath::RandRange(0, AvailableRecipes.Num() - 1)];

	FOrder NewOrder;
	NewOrder.RecipeName = ChosenRecipe;
	NewOrder.TimeRemaining = MaxOrderTime;
	NewOrder.TargetTable = TargetTable;

	ActiveOrders.Add(NewOrder);

	UE_LOG(LogTemp, Log, TEXT("New Order: %s for table %s"), *ChosenRecipe.ToString(), *TargetTable->GetName());
}

bool AOrderManager::TryCompleteOrder(FName RecipeName, ATableActor* Table)
{
	for (FOrder& Order : ActiveOrders)
	{
		if (!Order.bCompleted && Order.RecipeName == RecipeName && Order.TargetTable == Table)
		{
			Order.bCompleted = true;
			UE_LOG(LogTemp, Log, TEXT("Order completed: %s"), *RecipeName.ToString());
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Invalid order delivery: %s"), *RecipeName.ToString());
	return false;
}
