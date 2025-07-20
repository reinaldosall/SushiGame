#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SushiGameMode.generated.h"

UCLASS()
class SUSHIGAME_API ASushiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASushiGameMode();

protected:
	
	virtual void BeginPlay() override;
};
