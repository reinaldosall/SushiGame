#include "SushiGameState.h"
#include "Net/UnrealNetwork.h"
#include "SushiPlayerController.h"
#include "PlayerStatusWidget.h"

ASushiGameState::ASushiGameState()
{
	GlobalScore = 0;
}

int32 ASushiGameState::GetGlobalScore() const
{
	return GlobalScore;
}

void ASushiGameState::OnRep_GlobalScore()
{
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

void ASushiGameState::AddGlobalScore(int32 Amount)
{
	if (HasAuthority())
	{
		GlobalScore += Amount;
		OnRep_GlobalScore(); // força update no servidor também
	}
}
void ASushiGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASushiGameState, GlobalScore);
}
