#include "OrderManager.h"
#include "OrderHUDWidget.h"
#include "SushiGameState.h"
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

	// Prevents to generate orders when at lobby
	ASushiGameState* GS = GetWorld() ? GetWorld()->GetGameState<ASushiGameState>() : nullptr;
	if (!GS || GS->GetMatchState() != EMatchState::InGame) return;
	
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
	// Make sure there are recipes available to draft
	if (AvailableRecipes.Num() == 0) return;

	// Find all tables on level
	TArray<AActor*> FoundTables;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATableActor::StaticClass(), FoundTables);

	if (FoundTables.Num() == 0) return;

	// Filters only tables that do not yet have an active order
	TArray<ATableActor*> FreeTables;
	for (AActor* Actor : FoundTables)
	{
		if (ATableActor* Table = Cast<ATableActor>(Actor))
		{
			bool bHasOrder = false;
			for (const FOrder& Order : ActiveOrders)
			{
				if (!Order.bCompleted && Order.TargetTable == Table)
				{
					bHasOrder = true;
					break;
				}
			}

			if (!bHasOrder)
			{
				FreeTables.Add(Table);
			}
		}
	}

	// If all tables are busy order is not generated
	if (FreeTables.Num() == 0) return;

	// Choose a free table and a random recipe
	ATableActor* TargetTable = FreeTables[FMath::RandRange(0, FreeTables.Num() - 1)];
	FName ChosenRecipe = AvailableRecipes[FMath::RandRange(0, AvailableRecipes.Num() - 1)];

	// Create new order
	FOrder NewOrder;
	NewOrder.RecipeName = ChosenRecipe;
	NewOrder.TimeRemaining = MaxOrderTime;
	NewOrder.TargetTable = TargetTable;
	NewOrder.bCompleted = false;

	// Updates table's floating text with recipe name
	if (TargetTable)
	{
		TargetTable->UpdateFloatingOrderText(ChosenRecipe);
	}

	// Add the new order to the active list
	ActiveOrders.Add(NewOrder);

	// Refresh HUD on clients
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