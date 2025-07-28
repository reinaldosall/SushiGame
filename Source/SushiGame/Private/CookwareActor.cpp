#include "CookwareActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "SushiPlayerCharacter.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

ACookwareActor::ACookwareActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	ProgressWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressWidget"));
	ProgressWidget->SetupAttachment(RootComponent);
	ProgressWidget->SetWidgetSpace(EWidgetSpace::Screen);
	ProgressWidget->SetDrawSize(FVector2D(200.f, 50.f));
	ProgressWidget->SetRelativeLocation(FVector(0, 0, 100));
}

void ACookwareActor::BeginPlay()
{
	Super::BeginPlay();
	UpdateProgressUI();
	if (!HasAuthority())
	{
		FTimerHandle TempHandle;
		GetWorld()->GetTimerManager().SetTimer(TempHandle, [this]()
		{
			UpdateProgressUI();
		}, 0.3f, false);
	}
}

void ACookwareActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCooking)
	{
		if (HasAuthority())
		{
			CookingElapsedTime += DeltaTime;
		}
		UpdateProgressUI();

		if (HasAuthority() && CookingElapsedTime >= CookingDuration)
		{
			FinishCooking();
		}
	}
}

void ACookwareActor::OnInteract(ASushiPlayerCharacter* Player)
{
	if (!HasAuthority() || !Player) return;

	if (bIsCooking) return;

	if (SharedRecipeProgress < 3)
	{
		SharedRecipeProgress++;
		OnRep_RecipeProgress();	
		Player->SetRecipeProgress(SharedRecipeProgress);
	}
	else if (SharedRecipeProgress == 3)
	{
		StartCooking(Player);
	}
	else if (SharedRecipeProgress == 4)
	{
	 
		SharedRecipeProgress = 0;
		OnRep_RecipeProgress();
	}

	Multicast_UpdateProgress();
}

void ACookwareActor::StartCooking(ASushiPlayerCharacter* Player)
{
	bIsCooking = true;
	CookingElapsedTime = 0.f;
	LockedPlayer = Player;
	OnRep_IsCooking();
}

void ACookwareActor::FinishCooking()
{
	bIsCooking = false;
	SharedRecipeProgress = 4; // Estado "Done"
	if (LockedPlayer)
	{
		LockedPlayer->SetRecipeProgress(SharedRecipeProgress);
	}
	OnRep_IsCooking();
	OnRep_RecipeProgress();	
	Multicast_UpdateProgress();
}

void ACookwareActor::OnRep_RecipeProgress()
{
	UpdateProgressUI();
}

void ACookwareActor::OnRep_IsCooking()
{
	UpdateProgressUI();
}

void ACookwareActor::Multicast_UpdateProgress_Implementation()
{
	UpdateProgressUI();
}

void ACookwareActor::UpdateProgressUI()
{
	if (!ProgressWidget) return;
	ProgressWidget->SetVisibility(true); // Reforça que está visível
	ProgressWidget->SetHiddenInGame(false);

	UUserWidget* Widget = ProgressWidget->GetUserWidgetObject();
	if (!Widget) return;

	UTextBlock* Text = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("ProgressText")));
	UProgressBar* Bar = Cast<UProgressBar>(Widget->GetWidgetFromName(TEXT("ProgressBar")));

	FString Status = TEXT("Idle");

	if (bIsCooking)
	{
		Status = TEXT("Cooking");
		if (Bar)
		{
			Bar->SetVisibility(ESlateVisibility::Visible);
			Bar->SetPercent(CookingElapsedTime / CookingDuration);
		}
	}
	else if (SharedRecipeProgress == 4)
	{
		Status = TEXT("Done");
		if (Bar) Bar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		switch (SharedRecipeProgress)
		{
		case 1: Status = TEXT("Sliced"); break;
		case 2: Status = TEXT("Rolled"); break;
		case 3: Status = TEXT("Ready to Cook"); break;
		default: break;
		}
		if (Bar) Bar->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Text)
	{
		Text->SetText(FText::FromString(Status));
	}
}

void ACookwareActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACookwareActor, SharedRecipeProgress);
	DOREPLIFETIME(ACookwareActor, CookingElapsedTime);
	DOREPLIFETIME(ACookwareActor, bIsCooking);
}

void ACookwareActor::OnRep_CookingElapsedTime()
{
	UpdateProgressUI();
}