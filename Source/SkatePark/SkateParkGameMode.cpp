// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateParkGameMode.h"
#include "PlayerCharacter/SkateParkCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "SkatePark/PlayerCharacter/SkateCharacterController.h"
#include "SkatePark/PlayerState/SkatePlayerState.h"
#include "SkatePark/GameStates/SkateParkGameState.h"

ASkateParkGameMode::ASkateParkGameMode()
{

}

void ASkateParkGameMode::PointsAcquired(ASkateCharacterController* PlayerController, int32 ScoreAmount)
{
	if (PlayerController == nullptr) return;

	//Get Player State
	ASkatePlayerState* PlayerState = Cast<ASkatePlayerState>(PlayerController->PlayerState);

	//Get Game State
	ASkateParkGameState* SkateParkGameState = GetGameState<ASkateParkGameState>();

	if (PlayerState && SkateParkGameState)
	{
		//Update player state score
		PlayerState->AddToScore(ScoreAmount);
		//Update top socrer if necesary
		SkateParkGameState->UpdateTopScore(PlayerState);
	}
}
