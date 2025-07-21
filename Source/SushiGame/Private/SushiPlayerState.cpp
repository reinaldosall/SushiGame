#include "SushiPlayerState.h"
#include "Net/UnrealNetwork.h"

ASushiPlayerState::ASushiPlayerState()
{
	PlayerScore = 0;
	bReplicates = true;
}

void ASushiPlayerState::AddScore(int32 Amount)
{
	if (HasAuthority())
	{
		PlayerScore += Amount;
	}
}

void ASushiPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASushiPlayerState, PlayerScore);
}
