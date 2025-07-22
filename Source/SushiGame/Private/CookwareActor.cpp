#include "CookwareActor.h"
#include "IngredientActor.h"
#include "SushiPlayerCharacter.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACookwareActor::ACookwareActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CookwareMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CookwareMesh"));
	RootComponent = CookwareMesh;

	ProgressWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressWidget"));
	ProgressWidget->SetupAttachment(RootComponent);
	ProgressWidget->SetWidgetSpace(EWidgetSpace::Screen);
	ProgressWidget->SetDrawSize(FVector2D(150.f, 40.f));
	ProgressWidget->SetRelativeLocation(FVector(0, 0, 120.f)); 
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
	UE_LOG(LogTemp, Warning, TEXT("CookwareActor::OnInteract called"));
	if (!Ingredient) return;

	CurrentIngredient = Ingredient;
	UpdateProgressWidget(); // atualiza UI sempre que tocar

	UE_LOG(LogTemp, Warning, TEXT("Ingredient passed to cookware: %s"), *Ingredient->GetName());

	EIngredientState CurrentState = Ingredient->GetIngredientState();

	switch (CurrentState)
	{
	case EIngredientState::Raw:
		Ingredient->SetIngredientState(EIngredientState::Sliced);
		UE_LOG(LogTemp, Warning, TEXT("Changed state to: %d"), (int32)Ingredient->GetIngredientState());

		break;
	case EIngredientState::Sliced:
		Ingredient->SetIngredientState(EIngredientState::Rolled);
		UE_LOG(LogTemp, Warning, TEXT("Changed state to: %d"), (int32)Ingredient->GetIngredientState());

		break;
	case EIngredientState::Rolled:
		Ingredient->SetIngredientState(EIngredientState::Finished);
		UE_LOG(LogTemp, Warning, TEXT("Changed state to: %d"), (int32)Ingredient->GetIngredientState());

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
	
	CurrentIngredient = Ingredient;
	UpdateProgressWidget();
}

void ACookwareActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACookwareActor, CurrentIngredient);
	DOREPLIFETIME(ACookwareActor, bIsProcessing);
}

void ACookwareActor::OnRep_CurrentIngredient()
{
	UpdateProgressWidget();
}

void ACookwareActor::UpdateProgressWidget()
{
	if (!CurrentIngredient || !ProgressWidget) return;

	UUserWidget* Widget = ProgressWidget->GetUserWidgetObject();
	if (!Widget) return;

	if (UTextBlock* Text = Cast<UTextBlock>(Widget->GetWidgetFromName("ProgressText")))
	{
		FString StateStr;

		switch (CurrentIngredient->GetIngredientState())
		{
		case EIngredientState::Raw:
			StateStr = "Raw";
			break;
		case EIngredientState::Sliced:
			StateStr = "Sliced";
			break;
		case EIngredientState::Rolled:
			StateStr = "Rolled";
			break;
		case EIngredientState::Finished:
			StateStr = "Complete";
			break;
		default:
			StateStr = "Unknown";
			break;
		}

		Text->SetText(FText::FromString(StateStr));
	}
}
