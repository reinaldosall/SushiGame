#include "CookwareActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "SushiPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

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

	bIsCooking = false;
	CookingProgress = 0.f;
	CookingPlayer = nullptr;
}

void ACookwareActor::BeginPlay()
{
	Super::BeginPlay();
	UpdateProgressWidget("Idle", 0.f);
}

void ACookwareActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCooking && CookingPlayer && CookingPlayer->IsLocallyControlled())
	{
		CookingProgress += DeltaTime;
		const float Percent = FMath::Clamp(CookingProgress / CookingDuration, 0.f, 1.f);
		UpdateProgressWidget("Cooking", Percent);

		if (CookingProgress >= CookingDuration)
		{
			bIsCooking = false;
			CookingProgress = 0.f;

			if (CookingPlayer)
			{
				CookingPlayer->RecipeProgress = 3; // Done
			}

			UpdateProgressWidget("Done", 1.f);

			// Reset visual after 1 second
			GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &ACookwareActor::ResetProgressWidget, 1.0f, false);

			CookingPlayer = nullptr;
		}
	}
}

void ACookwareActor::OnInteract(ASushiPlayerCharacter* Player)
{
	if (!HasAuthority() || !Player) return;

	if (bIsCooking) return;

	if (Player->RecipeProgress >= 3)
	{
		Player->RecipeProgress = 4; // Complete
		return;
	}

	if (Player->RecipeProgress == 2)
	{
		// Start cooking
		CookingPlayer = Player;
		bIsCooking = true;
		CookingProgress = 0.f;
		return;
	}

	// Advance preparation stage
	Player->RecipeProgress++;
}

void ACookwareActor::UpdateProgressWidget(const FString& StatusText, float Percent)
{
	if (!ProgressWidget || !CookingPlayer || !CookingPlayer->IsLocallyControlled()) return;

	if (UUserWidget* Widget = ProgressWidget->GetUserWidgetObject())
	{
		if (UTextBlock* Text = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("ProgressText"))))
		{
			Text->SetText(FText::FromString(StatusText));
		}

		if (UProgressBar* Bar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("ProgressBar"))))
		{
			if (StatusText == "Cooking")
			{
				Bar->SetVisibility(ESlateVisibility::Visible);
				Bar->SetPercent(Percent);
			}
			else
			{
				Bar->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void ACookwareActor::ResetProgressWidget()
{
	UpdateProgressWidget("Idle", 0.f);
}
