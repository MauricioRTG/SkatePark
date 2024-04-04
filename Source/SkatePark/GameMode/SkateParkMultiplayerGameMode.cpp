// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateParkMultiplayerGameMode.h"
#include "SkatePark/PlayerCharacter/SkateCharacterController.h"
#include "SkatePark/PlayerState/SkatePlayerState.h"
#include "SkatePark/GameStates/SkateParkGameState.h"

void ASkateParkMultiplayerGameMode::PointsAcquired(ASkateCharacterController* PlayerController, int32 ScoreAmount)
{
	if (PlayerController == nullptr) return;

	//Get Player State
	ASkatePlayerState* PlayerState = Cast<ASkatePlayerState>(PlayerController->PlayerState);

	//Get Game State
	//ASkateParkGameState* SkateParkGameState = GetGameState<ASkateParkGameState>();

	if (PlayerState)
	{
		//Update player state score
		PlayerState->AddToScore(ScoreAmount);
		//Update top socrer if necesary
		//SkateParkGameState->UpdateTopScore(PlayerState);
	}
}

