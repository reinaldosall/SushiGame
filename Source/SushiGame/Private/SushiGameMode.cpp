#include "SushiGameMode.h"
#include "SushiPlayerController.h"
#include "SushiPlayerState.h"
#include "SushiGameState.h"
#include "UObject/ConstructorHelpers.h"

ASushiGameMode::ASushiGameMode()
{
	GameStateClass = ASushiGameState::StaticClass();
	PlayerStateClass = ASushiPlayerState::StaticClass();
	PlayerControllerClass = ASushiPlayerController::StaticClass();

	// Set default pawn from Blueprint
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Assets/Blueprints/Characters/BP_SushiGameCharacter"));
	if (PlayerPawnBPClass.Class)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find BP_SushiGameCharacter"));
	}
}

void ASushiGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Only host sets match state
	if (HasAuthority())
	{
		if (ASushiGameState* GS = GetGameState<ASushiGameState>())
		{
			FString MapName = GetWorld()->GetMapName();
			MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

			if (MapName == "Lvl_Restaurant")
			{
				GS->SetMatchState(EMatchState::InGame);
			}
			else
			{
				GS->SetMatchState(EMatchState::Lobby);
			}
		}
	}
}