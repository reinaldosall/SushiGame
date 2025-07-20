#include "IngredientActor.h"
#include "Net/UnrealNetwork.h"

AIngredientActor::AIngredientActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Trace hit check
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

	IngredientState = EIngredientState::Raw;
}

void AIngredientActor::BeginPlay()
{
	Super::BeginPlay();
	UpdateVisualForState();
}

void AIngredientActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIngredientActor::OnInteract()
{
	// For testing: advance state manually
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
	// Placeholder: for change meshes based on actor state
	UE_LOG(LogTemp, Log, TEXT("Ingredient changed to state: %d"), static_cast<int32>(IngredientState));
}

void AIngredientActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIngredientActor, IngredientState);
}