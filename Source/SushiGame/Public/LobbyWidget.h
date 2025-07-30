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
	virtual void NativeConstruct() override;

	// Start button widget
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	// Optional: Title text
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* LobbyTitleText;

	// Optional: Text inside the button
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* StartButtonText;

	// Called when Start button is clicked
	UFUNCTION()
	void HandleStartClicked();
};
