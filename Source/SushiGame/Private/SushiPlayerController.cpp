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

	if (IsLocalController())
	{
		// Show lobby HUD
		if (LobbyWidgetClass)
		{
			LobbyWidgetInstance = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
			if (LobbyWidgetInstance)
			{
				LobbyWidgetInstance->AddToViewport(50);
				SetInputMode(FInputModeUIOnly());
				bShowMouseCursor = true;
			}
		}
	}

	// Sync current match state (in case of late join)
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
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		break;

	case EMatchState::InGame:
		HideLobby();
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;

		// Show player HUD only when game starts
		if (!PlayerStatusWidgetInstance && PlayerStatusWidgetClass && IsLocalController())
		{
			PlayerStatusWidgetInstance = CreateWidget<UPlayerStatusWidget>(this, PlayerStatusWidgetClass);
			if (PlayerStatusWidgetInstance)
			{
				PlayerStatusWidgetInstance->AddToViewport(10);
			}
		}
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
