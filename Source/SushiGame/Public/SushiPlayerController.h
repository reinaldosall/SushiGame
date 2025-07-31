#pragma once

#include "CoreMinimal.h"
#include "SushiGameState.h"
#include "GameFramework/PlayerController.h"
#include "SushiPlayerController.generated.h"

class UPlayerStatusWidget;
class ULobbyWidget;

UCLASS()
class SUSHIGAME_API ASushiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASushiPlayerController();
	virtual void BeginPlay() override;

	// Reacts to replicated match state
	void HandleMatchState(EMatchState NewState);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerStatusWidget> PlayerStatusWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

public:
	UPROPERTY()
	UPlayerStatusWidget* PlayerStatusWidgetInstance;

	UPROPERTY()
	ULobbyWidget* LobbyWidgetInstance;

	void ShowLobby();
	void HideLobby();

	// Victory/Defeat UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> VictoryWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DefeatWidgetClass;

	// UI instances
	UUserWidget* VictoryWidgetInstance = nullptr;
	UUserWidget* DefeatWidgetInstance = nullptr;
};
