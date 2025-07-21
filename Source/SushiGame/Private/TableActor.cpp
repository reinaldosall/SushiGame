#include "TableActor.h"

ATableActor::ATableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMesh"));
	TableMesh->SetupAttachment(Root);
}

void ATableActor::BeginPlay()
{
	Super::BeginPlay();
}