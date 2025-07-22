#include "IngredientActor.h"

#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"

AIngredientActor::AIngredientActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicates(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	IngredientState = EIngredientState::Raw;
	IngredientType = FName("Default"); 

	//Widget 
	IngredientLabel = CreateDefaultSubobject<UWidgetComponent>(TEXT("IngredientLabel"));
	IngredientLabel->SetupAttachment(RootComponent);
	IngredientLabel->SetWidgetSpace(EWidgetSpace::Screen);
	IngredientLabel->SetDrawSize(FVector2D(100.f, 40.f));
	IngredientLabel->SetRelativeLocation(FVector(0, 0, 100.f));
}

void AIngredientActor::BeginPlay()
{
	Super::BeginPlay();
	UpdateVisualForState();
	
	if (UUserWidget* Widget = IngredientLabel->GetUserWidgetObject())
	{
		if (UTextBlock* Text = Cast<UTextBlock>(Widget->GetWidgetFromName("IngredientText")))
		{
			Text->SetText(FText::FromName(IngredientType));
		}
	}
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
