#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SushiGameState.generated.h"

//Represents the current state of the match
UENUM(BlueprintType)
enum class EMatchState : uint8
{
	Lobby,
	InGame,
	Victory,
	Defeat
};

UCLASS()
class SUSHIGAME_API ASushiGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASushiGameState();

	// Global score shared by all players
	UPROPERTY(ReplicatedUsing = OnRep_GlobalScore)
	int32 GlobalScore;

	// Current state of the match
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	EMatchState MatchState;

	// Adds or Remove from the global score
	UFUNCTION(BlueprintCallable)
	void AddGlobalScore(int32 Amount);

	// Returns the current global score 
	UFUNCTION(BlueprintCallable)
	int32 GetGlobalScore() const;

	// Sets the current match state (server only)
	UFUNCTION(BlueprintCallable)
	void SetMatchState(EMatchState NewState);

	// Returns the current match state
	UFUNCTION(BlueprintPure)
	EMatchState GetMatchState() const { return MatchState; }

protected:
	// Called when GlobalScore is replicated 
	UFUNCTION()
	void OnRep_GlobalScore();

	// Called when MatchState is replicated
	UFUNCTION()
	void OnRep_MatchState();

	// Checks if game should end
	void CheckEndGameCondition();
	
	// Handles replication of properties 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};