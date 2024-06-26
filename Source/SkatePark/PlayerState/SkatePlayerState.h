// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SkatePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SKATEPARK_API ASkatePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void AddToScore(int32 ScoreAmount);
};
