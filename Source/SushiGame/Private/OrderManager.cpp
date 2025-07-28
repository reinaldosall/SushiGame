#include "OrderManager.h"
#include "OrderHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TableActor.h"
#include "Net/UnrealNetwork.h"

AOrderManager::AOrderManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	EditorIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorIcon"));
	RootComponent = EditorIcon;
}

void AOrderManager::BeginPlay()
{
	Super::BeginPlay();

	if (!IsNetMode(NM_DedicatedServer) && HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UOrderHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			UpdateHUD();
		}
	}
}

void AOrderManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		TimeSinceLastOrder += DeltaTime;

		if (TimeSinceLastOrder >= OrderInterval)
		{
			GenerateOrder();
			TimeSinceLastOrder = 0.0f;
		}

		for (FOrder& Order : ActiveOrders)
		{
			if (!Order.bCompleted)
			{
				Order.TimeRemaining -= DeltaTime;

				if (Order.TimeRemaining <= 0.0f)
				{
					Order.bCompleted = true;
					if (Order.TargetTable)
					{
						Order.TargetTable->ClearFloatingOrderText();
					}
					UE_LOG(LogTemp, Warning, TEXT("Order expired: %s"), *Order.RecipeName.ToString());
				}
			}
		}
	}

	UpdateHUD();
}

void AOrderManager::UpdateHUD()
{
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
	if (AvailableRecipes.Num() == 0) return;

	TArray<AActor*> FoundTables;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATableActor::StaticClass(), FoundTables);

	if (FoundTables.Num() == 0) return;

	ATableActor* TargetTable = Cast<ATableActor>(FoundTables[FMath::RandRange(0, FoundTables.Num() - 1)]);
	FName ChosenRecipe = AvailableRecipes[FMath::RandRange(0, AvailableRecipes.Num() - 1)];

	FOrder NewOrder;
	NewOrder.RecipeName = ChosenRecipe;
	NewOrder.TimeRemaining = MaxOrderTime;
	NewOrder.TargetTable = TargetTable;
	NewOrder.bCompleted = false;

	if (TargetTable)
	{
		TargetTable->UpdateFloatingOrderText(ChosenRecipe);
	}

	ActiveOrders.Add(NewOrder);
	OnRep_ActiveOrders();
}

void AOrderManager::OnRep_ActiveOrders()
{
	UpdateHUD();
}

EDeliveryResult AOrderManager::TryCompleteOrder(FName RecipeName, ATableActor* Table)
{
	for (FOrder& Order : ActiveOrders)
	{
		if (Order.bCompleted)
			continue;

		if (Order.RecipeName != RecipeName)
			continue;

		if (Order.TargetTable != Table)
			return EDeliveryResult::WrongTable;
		
		Order.bCompleted = true;
		Order.TargetTable->ClearFloatingOrderText();

		OnRep_ActiveOrders();
		return EDeliveryResult::Success;
	}

	return EDeliveryResult::WrongTable;
}

void AOrderManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOrderManager, ActiveOrders);
}
