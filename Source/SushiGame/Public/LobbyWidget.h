#pragma once

#include "CoreMinimal.h"
#include "SushiGameState.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SUSHIGAME_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Initializes bindings
	UFUNCTION(BlueprintCallable)
	void Setup();

	// Called when MatchState changes
	void OnMatchStateChanged(EMatchState NewState);

protected:
	virtual void NativeConstruct() override;

	// Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	// Optional text labels
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* LobbyTitleText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* HostButtonText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* JoinButtonText;

	// Click handlers
	UFUNCTION()
	void HandleHostClicked();

	UFUNCTION()
	void HandleJoinClicked();

	UFUNCTION()
	void HandleQuitClicked();
};
