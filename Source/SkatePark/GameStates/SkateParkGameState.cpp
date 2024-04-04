// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateParkGameState.h"
#include "Net/UnrealNetwork.h"
#include "SkatePark/PlayerState/SkatePlayerState.h"

void ASkateParkGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkateParkGameState, TopScoringPlayers);
}

void ASkateParkGameState::UpdateTopScore(class ASkatePlayerState* SkatePlayerState)
{
	//if there is not top scorer yet
	if (TopScoringPlayers.IsEmpty())
	{
		TopScoringPlayers.Add(SkatePlayerState);
		TopScore = SkatePlayerState->GetScore();
	}
	else if (SkatePlayerState->GetScore() == TopScore) //if score is the same as top scorer
	{
		TopScoringPlayers.AddUnique(SkatePlayerState); //Add player with other player
	}
	else if(SkatePlayerState->GetScore() > TopScore) //Score is greater than the current top scorer
	{
		TopScoringPlayers.Empty(); //Empty array to add new top scorer
		TopScoringPlayers.AddUnique(SkatePlayerState);
		TopScore = SkatePlayerState->GetScore();
	}

	//Pirint top scorer
	if (TopScoringPlayers.Num() > 0)
	{
		for (int32 i = 0; i < TopScoringPlayers.Num(); i++)
		{
			UE_LOG(LogTemp, Log, TEXT("Name: %s"), *TopScoringPlayers[i]->GetPlayerName());
			UE_LOG(LogTemp, Log, TEXT("Score: %f"), TopScoringPlayers[i]->GetScore());
		}
	}
}
