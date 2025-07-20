#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SushiPlayerController.generated.h"

UCLASS()
class SUSHIGAME_API ASushiPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
