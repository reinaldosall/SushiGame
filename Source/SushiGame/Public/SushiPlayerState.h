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

protected:
	UFUNCTION()
	void OnRep_PlayerScore();

public:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerScore, BlueprintReadOnly, Category = "Score")
	int32 PlayerScore;

	void AddScore(int32 Amount);

	virtual int32 GetScore() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};