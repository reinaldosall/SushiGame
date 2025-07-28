#include "SushiPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SushiPlayerController.h"
#include "PlayerStatusWidget.h"
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
		OnRep_PlayerScore(); // opcional: força HUD local do host a atualizar
	}
}

int32 ASushiPlayerState::GetScore() const
{
	return PlayerScore;
}

// void ASushiPlayerState::OnRep_PlayerScore()
// {
// 	// Atualiza HUD local do player
// 	if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
// 	{
// 		if (ASushiPlayerController* SPC = Cast<ASushiPlayerController>(PC))
// 		{
// 			if (SPC->PlayerStatusWidgetInstance)
// 			{
// 				SPC->PlayerStatusWidgetInstance->UpdateScore(FString::FromInt(PlayerScore));
// 			}
// 		}
// 	}
// }

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
