#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SushiPlayerController.generated.h"

class UPlayerStatusWidget;

UCLASS()
class SUSHIGAME_API ASushiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	ASushiPlayerController();
	
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerStatusWidget> PlayerStatusWidgetClass;

public:
	UPROPERTY()
	UPlayerStatusWidget* PlayerStatusWidgetInstance;

	FORCEINLINE void SetStatusWidgetClass(TSubclassOf<UPlayerStatusWidget> InClass) { PlayerStatusWidgetClass = InClass; }
};
