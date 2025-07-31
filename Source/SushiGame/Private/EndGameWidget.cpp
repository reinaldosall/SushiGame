#include "EndGameWidget.h"
#include "SushiGameState.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

void UEndGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UEndGameWidget::OnRestartClicked);

		// Hide restart button if not the host
		if (!GetOwningPlayer() || !GetOwningPlayer()->HasAuthority())
		{
			RestartButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UEndGameWidget::OnQuitClicked);
	}

	if (ResultText)
	{
		FString StateStr = "GAME OVER";

		if (const ASushiGameState* GS = GetWorld() ? GetWorld()->GetGameState<ASushiGameState>() : nullptr)
		{
			bool bIsHost = GetOwningPlayer() && GetOwningPlayer()->HasAuthority();

			switch (GS->GetMatchState())
			{
			case EMatchState::Victory:
				StateStr = bIsHost ? "YOU WIN!" : "YOU WIN!  Waiting for HOST to restart...";
				break;

			case EMatchState::Defeat:
				StateStr = bIsHost ? "YOU LOSE!" : "YOU LOSE!  Waiting for Host to restart...";
				break;

			default:
				break;
			}
		}

		ResultText->SetText(FText::FromString(StateStr));
	}
}

void UEndGameWidget::OnRestartClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (PC->HasAuthority())
		{
			// Only the host restarts the match
			UGameplayStatics::OpenLevel(PC, "Lvl_Lobby", true, "listen");
		}
	}
}

void UEndGameWidget::OnQuitClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
	}
}