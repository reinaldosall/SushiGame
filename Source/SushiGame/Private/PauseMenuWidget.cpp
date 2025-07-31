#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReturnToLobbyButton)
	{
		ReturnToLobbyButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnReturnToLobbyClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
	}
}

void UPauseMenuWidget::OnReturnToLobbyClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (PC->HasAuthority())
		{
			// Host: only one who restarts lobby
			UGameplayStatics::OpenLevel(PC, "Lvl_Lobby", true, "listen");
		}
		else
		{
			// Guest: shows only text
			if (UTextBlock* Status = Cast<UTextBlock>(GetWidgetFromName(TEXT("StatusText"))))
			{
				Status->SetText(FText::FromString("Waiting for host to restart..."));
			}
		}
	}
}

void UPauseMenuWidget::OnQuitClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
	}
}