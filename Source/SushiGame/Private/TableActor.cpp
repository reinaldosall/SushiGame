#include "TableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

ATableActor::ATableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMesh"));
	TableMesh->SetupAttachment(Root);

	TableOrderWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TableOrderWidget"));
	TableOrderWidget->SetupAttachment(Root);
	TableOrderWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TableOrderWidget->SetDrawSize(FVector2D(150.f, 40.f));
	TableOrderWidget->SetRelativeLocation(FVector(0, 0, 100.f));
}

void ATableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATableActor::UpdateFloatingOrderText(const FName& OrderName)
{
	CurrentOrderName = OrderName;
	OnRep_OrderName();
}

void ATableActor::ClearFloatingOrderText()
{
	CurrentOrderName = NAME_None;
	OnRep_OrderName();
}

void ATableActor::OnRep_OrderName()
{
	if (!TableOrderWidget) return;

	if (UUserWidget* Widget = TableOrderWidget->GetUserWidgetObject())
	{
		if (UTextBlock* Text = Cast<UTextBlock>(Widget->GetWidgetFromName("OrderText")))
		{
			Text->SetText(CurrentOrderName.IsNone() ? FText::FromString(TEXT("-")) : FText::FromName(CurrentOrderName));
		}
	}
}

void ATableActor::SetFeedbackText(const FString& Text)
{
	if (!TableOrderWidget) return;

	if (UUserWidget* Widget = TableOrderWidget->GetUserWidgetObject())
	{
		if (UTextBlock* TextBlock = Cast<UTextBlock>(Widget->GetWidgetFromName("OrderText")))
		{
			FeedbackVisualText = Text;
			OnRep_FeedbackVisualText();
		}
	}

	// clean after 1sec
	GetWorld()->GetTimerManager().SetTimer(FeedbackResetTimer, this, &ATableActor::ResetFeedbackText, 1.0f, false);
}

void ATableActor::ResetFeedbackText()
{
	FeedbackVisualText = TEXT("");
	OnRep_OrderName(); 
	OnRep_FeedbackVisualText(); // Clean text on guests 
	Multicast_RefreshOrderName(); // Refresh recipe name
}

void ATableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATableActor, CurrentOrderName);
	DOREPLIFETIME(ATableActor, FeedbackVisualText);
}

void ATableActor::OnRep_FeedbackVisualText()
{
	if (!TableOrderWidget) return;

	if (UUserWidget* Widget = TableOrderWidget->GetUserWidgetObject())
	{
		if (UTextBlock* TextBlock = Cast<UTextBlock>(Widget->GetWidgetFromName("OrderText")))
		{
			if (!FeedbackVisualText.IsEmpty())
			{
				TextBlock->SetText(FText::FromString(FeedbackVisualText));
			}
			else
			{
				TextBlock->SetText(CurrentOrderName.IsNone() ? FText::FromString(TEXT("-")) : FText::FromName(CurrentOrderName));
			}
		}
	}
}

void ATableActor::Multicast_RefreshOrderName_Implementation()
{
	OnRep_OrderName();
}