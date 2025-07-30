#include "SushiGameState.h"
#include "Net/UnrealNetwork.h"
#include "SushiPlayerController.h"
#include "PlayerStatusWidget.h"

ASushiGameState::ASushiGameState()
{
	GlobalScore = 0;
	MatchState = EMatchState::Lobby;
	bReplicates = true;
}

int32 ASushiGameState::GetGlobalScore() const
{
	return GlobalScore;
}

void ASushiGameState::AddGlobalScore(int32 Amount)
{
	if (HasAuthority())
	{
		GlobalScore += Amount;
		OnRep_GlobalScore();
	}
}

void ASushiGameState::OnRep_GlobalScore()
{
	// Update local HUD for each player
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ASushiPlayerController* PC = Cast<ASushiPlayerController>(It->Get()))
		{
			if (PC->IsLocalController() && PC->PlayerStatusWidgetInstance)
			{
				PC->PlayerStatusWidgetInstance->UpdateScore(FString::Printf(TEXT("%d"), GlobalScore));
			}
		}
	}
}

void ASushiGameState::SetMatchState(EMatchState NewState)
{
	if (HasAuthority() && MatchState != NewState)
	{
		MatchState = NewState;
		OnRep_MatchState(); // local update for server
	}
}

void ASushiGameState::OnRep_MatchState()
{
	// Notify each player controller to handle state change
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ASushiPlayerController* PC = Cast<ASushiPlayerController>(It->Get()))
		{
			PC->HandleMatchState(MatchState);
		}
	}
}

void ASushiGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASushiGameState, GlobalScore);
	DOREPLIFETIME(ASushiGameState, MatchState);
}