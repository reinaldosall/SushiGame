#include "SushiLobbyGameMode.h"
#include "SushiGameState.h"
#include "SushiPlayerController.h"

ASushiLobbyGameMode::ASushiLobbyGameMode()
{
	GameStateClass = ASushiGameState::StaticClass();
	PlayerControllerClass = ASushiPlayerController::StaticClass();
	DefaultPawnClass = nullptr; 
}
