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
	// Called to setup the widget (binds delegates)
	UFUNCTION(BlueprintCallable)
	void Setup();

	// Called when match state changes
	void OnMatchStateChanged(EMatchState NewState);

protected:
	// UI Bindings
	virtual void NativeConstruct() override;

	// Start button widget
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;
	
	// Custom Title text
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* LobbyTitleText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* HostButtonText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* JoinButtonText;
	
	// Called when Start button is clicked
	// UFUNCTION()
	// void HandleStartClicked();

	// Host game
	UFUNCTION()
	void HandleHostClicked();

	// Join game
	UFUNCTION()
	void HandleJoinClicked();

	// UFUNCTION()
	// void HostGame();
	//
	// UFUNCTION()
	// void JoinGame();
};
