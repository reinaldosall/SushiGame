#include "CookwareActor.h"
#include "IngredientActor.h"
#include "SushiPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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

void ACookwareActor::OnProcessingComplete()
{ 
	UE_LOG(LogTemp, Log, TEXT("Processing completed in Cookware."));
}

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

	ASushiPlayerCharacter* Player = Cast<ASushiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		if (Player->HeldRecipe.IsNone())
		{
			Player->HeldRecipe = Ingredient->IngredientType;
			Player->RecipeProgress = 1;
			UE_LOG(LogTemp, Warning, TEXT("Player started new recipe: %s"), *Player->HeldRecipe.ToString());
		}
		else if (Player->HeldRecipe == Ingredient->IngredientType)
		{
			if (Player->RecipeProgress < 3)
			{
				Player->RecipeProgress++;
				UE_LOG(LogTemp, Warning, TEXT("Progressed recipe to step %d"), Player->RecipeProgress);
			}
		}
	}
}

void ACookwareActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACookwareActor, CurrentIngredient);
	DOREPLIFETIME(ACookwareActor, bIsProcessing);
}
