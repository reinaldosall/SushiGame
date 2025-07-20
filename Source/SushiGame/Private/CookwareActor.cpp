#include "CookwareActor.h"
#include "IngredientActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

ACookwareActor::ACookwareActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ACookwareActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACookwareActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
/*
void ACookwareActor::OnInteract(AIngredientActor* Ingredient)
{
	/*if (!HasAuthority() || bIsProcessing || !Ingredient) return;

	StartProcessing(Ingredient);
*/
void ACookwareActor::OnInteract(AIngredientActor* Ingredient)
{
	if (!Ingredient) return;

	EIngredientState CurrentState = Ingredient->GetIngredientState();

	UE_LOG(LogTemp, Warning, TEXT("Cookware interacted! Ingredient state: %d"), (int32)CurrentState);
	 
	switch (CurrentState)
	{
	case EIngredientState::Raw:
		Ingredient->SetIngredientState(EIngredientState::Sliced);
		break;
	case EIngredientState::Sliced:
		Ingredient->SetIngredientState(EIngredientState::Rolled);
		break;
	default:
		break;
	}
}


void ACookwareActor::StartProcessing(AIngredientActor* Ingredient)
{
	bIsProcessing = true;
	CurrentIngredient = Ingredient;

	GetWorldTimerManager().SetTimer(ProcessingTimerHandle, this, &ACookwareActor::OnProcessingComplete, ProcessingTime, false);
}

void ACookwareActor::OnProcessingComplete()
{
	if (CurrentIngredient)
	{
		// Jumps to next state
		CurrentIngredient->OnInteract(); // Reuse ingredient logic
	}

	bIsProcessing = false;
	CurrentIngredient = nullptr;
}

void ACookwareActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACookwareActor, CurrentIngredient);
	DOREPLIFETIME(ACookwareActor, bIsProcessing);
}