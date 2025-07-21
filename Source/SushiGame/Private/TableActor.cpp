#include "TableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"

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
	TableOrderWidget->SetRelativeLocation(FVector(0, 0, 100.f));

	bReplicates = true;
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

void ATableActor::OnRep_OrderName()
{
	if (!TableOrderWidget) return;

	UUserWidget* Widget = TableOrderWidget->GetUserWidgetObject();
	if (!Widget) return;

	if (UTextBlock* Text = Cast<UTextBlock>(Widget->GetWidgetFromName("OrderText")))
	{
		Text->SetText(CurrentOrderName.IsNone() ? FText::FromString(TEXT("-")) : FText::FromName(CurrentOrderName));
	}
}

void ATableActor::ClearFloatingOrderText()
{
	CurrentOrderName = NAME_None;
	OnRep_OrderName();
}

void ATableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATableActor, CurrentOrderName);
}
