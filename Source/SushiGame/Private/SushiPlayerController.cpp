#include "SushiPlayerController.h"
#include "LobbyWidget.h"
#include "PlayerStatusWidget.h"
#include "SushiGameState.h"
#include "Blueprint/UserWidget.h"

ASushiPlayerController::ASushiPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPlayerStatusWidget> StatusClass(TEXT("/Game/Assets/Blueprints/Widgets/WBP_PlayerStatus"));
	if (StatusClass.Succeeded())
	{
		PlayerStatusWidgetClass = StatusClass.Class;
	}

	static ConstructorHelpers::FClassFinder<ULobbyWidget> LobbyClass(TEXT("/Game/Assets/Blueprints/Widgets/WBP_Lobby"));
	if (LobbyClass.Succeeded())
	{
		LobbyWidgetClass = LobbyClass.Class;
	}
}

void ASushiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;

	// Create and show status HUD
	if (PlayerStatusWidgetClass)
	{
		PlayerStatusWidgetInstance = CreateWidget<UPlayerStatusWidget>(this, PlayerStatusWidgetClass);
		if (PlayerStatusWidgetInstance)
		{
			PlayerStatusWidgetInstance->AddToViewport(10);
		}
	}

	// Create lobby widget
	if (LobbyWidgetClass)
	{
		LobbyWidgetInstance = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
	}

	// Sync current match state
	if (const ASushiGameState* GS = GetWorld()->GetGameState<ASushiGameState>())
	{
		HandleMatchState(GS->GetMatchState());
	}
}

void ASushiPlayerController::HandleMatchState(EMatchState NewState)
{
	switch (NewState)
	{
	case EMatchState::Lobby:
		ShowLobby();
		break;

	case EMatchState::InGame:
		HideLobby();
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
		break;

	case EMatchState::Victory:
	case EMatchState::Defeat:
		// Future: show end screen
		break;
	}
}

void ASushiPlayerController::ShowLobby()
{
	if (LobbyWidgetInstance && !LobbyWidgetInstance->IsInViewport())
	{
		LobbyWidgetInstance->AddToViewport(20);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void ASushiPlayerController::HideLobby()
{
	if (LobbyWidgetInstance)
	{
		LobbyWidgetInstance->RemoveFromParent();
	}
}