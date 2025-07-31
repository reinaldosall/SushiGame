#include "SushiPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SushiPlayerCharacter.h"

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
		OnRep_PlayerScore();
	}
}

int32 ASushiPlayerState::GetScore() const
{
	return PlayerScore;
}

void ASushiPlayerState::OnRep_PlayerScore()
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			if (ASushiPlayerCharacter* Character = Cast<ASushiPlayerCharacter>(Pawn))
			{
				Character->UpdatePlayerStatusUI(); // vai atualizar o ScoreText
			}
		}
	}
}

void ASushiPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASushiPlayerState, PlayerScore);
}