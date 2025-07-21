#include "IngredientActor.h"
#include "Net/UnrealNetwork.h"

AIngredientActor::AIngredientActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	IngredientState = EIngredientState::Raw;
	IngredientType = FName("Default"); // Pode configurar no editor para tipos reais
}

void AIngredientActor::BeginPlay()
{
	Super::BeginPlay();
	UpdateVisualForState();
}

void AIngredientActor::OnInteract()
{
	switch (IngredientState)
	{
	case EIngredientState::Raw:
		SetIngredientState(EIngredientState::Sliced);
		break;
	case EIngredientState::Sliced:
		SetIngredientState(EIngredientState::Rolled);
		break;
	case EIngredientState::Rolled:
		SetIngredientState(EIngredientState::Finished);
		break;
	default:
		break;
	}
}

void AIngredientActor::SetIngredientState(EIngredientState NewState)
{
	if (HasAuthority())
	{
		IngredientState = NewState;
		OnRep_IngredientState();

		UE_LOG(LogTemp, Warning, TEXT("Ingredient changed to state: %d"), (int32)NewState);
	}
}

void AIngredientActor::OnRep_IngredientState()
{
	UpdateVisualForState();
}

void AIngredientActor::UpdateVisualForState()
{
	// change mesh comes here
	UE_LOG(LogTemp, Log, TEXT("Visual update for state: %d"), static_cast<int32>(IngredientState));
}

void AIngredientActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIngredientActor, IngredientState);
}
