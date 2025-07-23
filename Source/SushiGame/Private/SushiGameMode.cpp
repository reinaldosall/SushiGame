#include "SushiGameMode.h"
#include "SushiPlayerController.h"
#include "SushiPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerStatusWidget.h"

ASushiGameMode::ASushiGameMode()
{
	PlayerStateClass = ASushiPlayerState::StaticClass();
	PlayerControllerClass = ASushiPlayerController::StaticClass();
}

void ASushiGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC || PC->GetPawn()) return;

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Assets/Blueprints/Characters/BP_SushiGameCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		FVector SpawnLocation = FVector(0, 0, 200);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PlayerPawnBPClass.Class, SpawnLocation, SpawnRotation, Params);
		if (NewPawn)
		{
			PC->Possess(NewPawn);
			UE_LOG(LogTemp, Warning, TEXT("Spawned and possessed BP_SushiGameCharacter: %s"), *NewPawn->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BP_SushiGameCharacter not found! Check path."));
	}
}
