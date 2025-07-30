#include "SushiPlayerController.h"

#include "LobbyWidget.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusWidget.h"
#include "SushiGameState.h"

void ASushiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		// Create and show player status HUD
		if (PlayerStatusWidgetClass)
		{
			PlayerStatusWidgetInstance = CreateWidget<UPlayerStatusWidget>(this, PlayerStatusWidgetClass);
			if (PlayerStatusWidgetInstance)
			{
				PlayerStatusWidgetInstance->AddToViewport(10);
			}
		}

		// Create lobby widget (but don't add yet)
		if (LobbyWidgetClass)
		{
			LobbyWidgetInstance = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
		}

		// Handle match state at join
		if (const ASushiGameState* GS = GetWorld()->GetGameState<ASushiGameState>())
		{
			HandleMatchState(GS->GetMatchState());
		}
	}

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

ASushiPlayerController::ASushiPlayerController()
{
	// Load status HUD
	static ConstructorHelpers::FClassFinder<UPlayerStatusWidget> StatusClass(TEXT("/Game/Assets/Blueprints/Widgets/WBP_PlayerStatus"));
	if (StatusClass.Succeeded())
	{
		PlayerStatusWidgetClass = StatusClass.Class;
	}

	// Load lobby widget
	static ConstructorHelpers::FClassFinder<ULobbyWidget> LobbyClass(TEXT("/Game/Assets/Blueprints/Widgets/WBP_Lobby"));
	if (LobbyClass.Succeeded())
	{
		LobbyWidgetClass = LobbyClass.Class;
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
	default:
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