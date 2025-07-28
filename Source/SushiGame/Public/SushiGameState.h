#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SushiGameState.generated.h"

UCLASS()
class SUSHIGAME_API ASushiGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ASushiGameState();

	UFUNCTION()
	void OnRep_GlobalScore(); // apenas declaração

	UFUNCTION(BlueprintCallable)
	void AddGlobalScore(int32 Amount);

	UFUNCTION(BlueprintCallable)
	int32 GetGlobalScore() const;

	UPROPERTY(ReplicatedUsing = OnRep_GlobalScore)
	int32 GlobalScore;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};