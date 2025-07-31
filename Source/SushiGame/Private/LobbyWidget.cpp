#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "SushiGameState.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Setup();
}

void ULobbyWidget::Setup()
{
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ULobbyWidget::HandleHostClicked);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ULobbyWidget::HandleJoinClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &ULobbyWidget::HandleQuitClicked);
	}
}

void ULobbyWidget::HandleHostClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		UGameplayStatics::OpenLevel(PC, "Lvl_Restaurant", true, "listen");
	}
}

void ULobbyWidget::HandleJoinClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->ClientTravel("127.0.0.1", TRAVEL_Absolute);
	}
}

void ULobbyWidget::HandleQuitClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
	}
}

void ULobbyWidget::OnMatchStateChanged(EMatchState NewState)
{
	if (NewState != EMatchState::Lobby)
	{
		this->RemoveFromParent();
	}
}
