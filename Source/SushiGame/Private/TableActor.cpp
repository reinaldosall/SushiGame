#include "TableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

ATableActor::ATableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMesh"));
	TableMesh->SetupAttachment(Root);

	TableOrderWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TableOrderWidget"));
	TableOrderWidget->SetupAttachment(Root);
	TableOrderWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TableOrderWidget->SetDrawSize(FVector2D(150.f, 40.f));
	TableOrderWidget->SetRelativeLocation(FVector(0, 0, 100.f)); // altura do widget
}

void ATableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATableActor::UpdateFloatingOrderText(const FName& OrderName)
{
	if (!TableOrderWidget) return;

	UUserWidget* Widget = TableOrderWidget->GetUserWidgetObject();
	if (!Widget) return;

	UTextBlock* Text = Cast<UTextBlock>(Widget->GetWidgetFromName("OrderText"));
	if (Text)
	{
		Text->SetText(FText::FromName(OrderName));
	}
}
void ATableActor::ClearFloatingOrderText()
{
	if (!TableOrderWidget) return;

	UUserWidget* Widget = TableOrderWidget->GetUserWidgetObject();
	if (!Widget) return;

	UTextBlock* Text = Cast<UTextBlock>(Widget->GetWidgetFromName("OrderText"));
	if (Text)
	{
		Text->SetText(FText::GetEmpty());
	}
}