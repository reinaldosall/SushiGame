#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SushiPlayerState.generated.h"

UCLASS()
class SUSHIGAME_API ASushiPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASushiPlayerState();

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Score")
	int32 PlayerScore;

	UFUNCTION(BlueprintCallable)
	void AddScore(int32 Amount);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
