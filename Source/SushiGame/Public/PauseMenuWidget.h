#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

UCLASS()
class SUSHIGAME_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnToLobbyButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UFUNCTION()
	void OnReturnToLobbyClicked();

	UFUNCTION()
	void OnQuitClicked();

	virtual void NativeConstruct() override;
};