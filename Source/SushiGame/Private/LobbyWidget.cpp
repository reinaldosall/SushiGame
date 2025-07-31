#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SushiGameState.h"
#include "GameFramework/PlayerController.h"


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
}

void ULobbyWidget::HandleHostClicked()
{
	// Starts the game as listen server
	//UGameplayStatics::OpenLevel(GetWorld(), FName("Lvl_Restaurant"), true, "listen");
	
	if (APlayerController* PC = GetOwningPlayer())
	{
		UGameplayStatics::OpenLevel(PC, "Lvl_Restaurant", true, "listen");
	}
}

void ULobbyWidget::HandleJoinClicked()
{
	// Joins the local host (for testing  127.0.0.1)
	//UGameplayStatics::OpenLevel(GetWorld(), FName("127.0.0.1"));
	
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->ClientTravel("127.0.0.1", TRAVEL_Absolute);
	}
}

// void ULobbyWidget::HostGame()
// {
// 	if (APlayerController* PC = GetOwningPlayer())
// 	{
// 		PC->ConsoleCommand("open Lvl_Restaurant?listen");
// 	}
// }
//
// void ULobbyWidget::JoinGame()
// {
// 	if (APlayerController* PC = GetOwningPlayer())
// 	{
// 		PC->ConsoleCommand("open 127.0.0.1");
// 	}
// }

void ULobbyWidget::OnMatchStateChanged(EMatchState NewState)
{
	if (NewState != EMatchState::Lobby)
	{
		this->RemoveFromParent();
	}
}