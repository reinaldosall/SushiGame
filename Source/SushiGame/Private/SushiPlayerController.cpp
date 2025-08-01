#include "SushiPlayerController.h"
#include "LobbyWidget.h"
#include "PauseMenuWidget.h"
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

	static ConstructorHelpers::FClassFinder<UUserWidget> VictoryClass(TEXT("/Game/Assets/Blueprints/Widgets/WBP_Victory"));
	if (VictoryClass.Succeeded())
	{
		VictoryWidgetClass = VictoryClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> DefeatClass(TEXT("/Game/Assets/Blueprints/Widgets/WBP_Defeat"));
	if (DefeatClass.Succeeded())
	{
		DefeatWidgetClass = DefeatClass.Class;
	}
	static ConstructorHelpers::FClassFinder<UPauseMenuWidget> PauseClass(TEXT("/Game/Assets/Blueprints/Widgets/WBP_PauseMenu"));
	if (PauseClass.Succeeded())
	{
		PauseMenuClass = PauseClass.Class;
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
		// Hide gameplay HUD
		if (PlayerStatusWidgetInstance)
		{
			PlayerStatusWidgetInstance->RemoveFromParent();
			PlayerStatusWidgetInstance = nullptr;
		}

		// Show Victory screen
		if (VictoryWidgetClass && !VictoryWidgetInstance)
		{
			VictoryWidgetInstance = CreateWidget<UUserWidget>(this, VictoryWidgetClass);
			if (VictoryWidgetInstance)
			{
				VictoryWidgetInstance->AddToViewport(100);
				SetInputMode(FInputModeUIOnly());
				bShowMouseCursor = true;
			}
		}
		break;
		
	case EMatchState::Defeat:
		// Hide gameplay HUD
		if (PlayerStatusWidgetInstance)
		{
			PlayerStatusWidgetInstance->RemoveFromParent();
			PlayerStatusWidgetInstance = nullptr;
		}

		// Show Defeat screen
		if (DefeatWidgetClass && !DefeatWidgetInstance)
		{
			DefeatWidgetInstance = CreateWidget<UUserWidget>(this, DefeatWidgetClass);
			if (DefeatWidgetInstance)
			{
				DefeatWidgetInstance->AddToViewport(100);
				SetInputMode(FInputModeUIOnly());
				bShowMouseCursor = true;
			}
		}
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

void ASushiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASushiPlayerController::TogglePauseMenu);
}

void ASushiPlayerController::TogglePauseMenu()
{
	if (!PauseMenuInstance && PauseMenuClass)
	{
		PauseMenuInstance = CreateWidget<UPauseMenuWidget>(this, PauseMenuClass);
		if (PauseMenuInstance)
		{
			PauseMenuInstance->AddToViewport(90);

			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			SetInputMode(InputMode);

			bShowMouseCursor = true;
		}
	}
	else if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void ASushiPlayerController::Client_ShowPenaltyMessage_Implementation(const FString& Message)
{
	if (PlayerStatusWidgetInstance)
	{
		PlayerStatusWidgetInstance->ShowTemporaryMessage(Message, 3.0f);
	}
}