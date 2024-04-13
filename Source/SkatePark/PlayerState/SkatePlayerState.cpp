// Fill out your copyright notice in the Description page of Project Settings.


#include "SkatePlayerState.h"

void ASkatePlayerState::AddToScore(int32 ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);
}
