#include "CookwareActor.h"
#include "IngredientActor.h"
#include "SushiPlayerCharacter.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
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
    if (!Ingredient || bIsCooking) return;

    APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (bIsDone && PlayerController == LockedPlayer)
    {
        Ingredient->SetIngredientState(EIngredientState::Finished);
        bIsDone = false;
        LockedPlayer = nullptr;
        CurrentIngredient = nullptr;
        UpdateProgressWidget();
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Ingredient passed to cookware: %s"), *Ingredient->GetName());
    EIngredientState CurrentState = Ingredient->GetIngredientState();

    switch (CurrentState)
    {
    case EIngredientState::Raw:
        Ingredient->SetIngredientState(EIngredientState::Sliced);
        break;
    case EIngredientState::Sliced:
        Ingredient->SetIngredientState(EIngredientState::Rolled);
        break;
    case EIngredientState::Rolled:
        bIsCooking = true;
        LockedPlayer = PlayerController;
        GetWorld()->GetTimerManager().SetTimer(CookingTimerHandle, this, &ACookwareActor::OnCookingFinished, 5.0f, false);
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
        }
        else if (Player->HeldRecipe == Ingredient->IngredientType)
        {
            if (Player->RecipeProgress < 3)
            {
                Player->RecipeProgress++;
            }
        }
    }

    CurrentIngredient = Ingredient;
    UpdateProgressWidget();
}

void ACookwareActor::OnCookingFinished()
{
    bIsCooking = false;
    bIsDone = true;
    UpdateProgressWidget();
}

void ACookwareActor::UpdateProgressWidget()
{
    if (!ProgressWidget) return;

    UUserWidget* Widget = ProgressWidget->GetUserWidgetObject();
    if (!Widget) return;

    UTextBlock* Text = Cast<UTextBlock>(Widget->GetWidgetFromName("ProgressText"));
    UProgressBar* Bar = Cast<UProgressBar>(Widget->GetWidgetFromName("ProgressBar"));

    FString StateStr = "Idle";

    if (bIsCooking)
    {
        StateStr = "Cooking";
        if (Bar) Bar->SetVisibility(ESlateVisibility::Visible);
    }
    else if (bIsDone)
    {
        StateStr = "Done";
        if (Bar) Bar->SetVisibility(ESlateVisibility::Hidden);
    }
    else if (CurrentIngredient)
    {
        switch (CurrentIngredient->GetIngredientState())
        {
        case EIngredientState::Raw:
            StateStr = "Raw"; break;
        case EIngredientState::Sliced:
            StateStr = "Sliced"; break;
        case EIngredientState::Rolled:
            StateStr = "Rolled"; break;
        case EIngredientState::Finished:
            StateStr = "Complete"; break;
        default:
            StateStr = "Unknown"; break;
        }
        if (Bar) Bar->SetVisibility(ESlateVisibility::Hidden);
    }

    if (Text)
    {
        Text->SetText(FText::FromString(StateStr));
    }
}

void ACookwareActor::OnRep_CurrentIngredient()
{
    UpdateProgressWidget();
}

void ACookwareActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACookwareActor, CurrentIngredient);
    DOREPLIFETIME(ACookwareActor, bIsProcessing);
}
