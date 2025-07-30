#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SushiGameState.h"
#include "GameFramework/PlayerController.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::HandleStartClicked);
	}
}

void ULobbyWidget::Setup()
{
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::HandleStartClicked);
	}
} 

void ULobbyWidget::HandleStartClicked()
{
	// Only the server (host) can change match state
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (PC->HasAuthority())
		{
			if (ASushiGameState* GS = Cast<ASushiGameState>(GetWorld()->GetGameState()))
			{
				GS->SetMatchState(EMatchState::InGame);
			}
		}
	}
}

void ULobbyWidget::OnMatchStateChanged(EMatchState NewState)
{
	if (NewState != EMatchState::Lobby)
	{
		this->RemoveFromParent();
	}
}