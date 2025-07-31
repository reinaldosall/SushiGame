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
	if (!HasAuthority()) return;

	GlobalScore += Amount;
	OnRep_GlobalScore();

	CheckEndGameCondition();
}

void ASushiGameState::OnRep_GlobalScore()
{
	// Updates HUD for local players
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
	if (!HasAuthority() || MatchState == NewState) return;

	MatchState = NewState;
	OnRep_MatchState(); // Local update for server
}

void ASushiGameState::OnRep_MatchState()
{
	// Notifies local controllers
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ASushiPlayerController* PC = Cast<ASushiPlayerController>(It->Get()))
		{
			if (PC->IsLocalController())
			{
				PC->HandleMatchState(MatchState);
			}
		}
	}
}void ASushiGameState::CheckEndGameCondition()
{
	const int32 WinScore = 500;
	const int32 LoseScore = -200;

	if (MatchState != EMatchState::InGame) return;

	if (GlobalScore >= WinScore)
	{
		SetMatchState(EMatchState::Victory);
	}
	else if (GlobalScore <= LoseScore)
	{
		SetMatchState(EMatchState::Defeat);
	}
}

void ASushiGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASushiGameState, GlobalScore);
	DOREPLIFETIME(ASushiGameState, MatchState);
}
