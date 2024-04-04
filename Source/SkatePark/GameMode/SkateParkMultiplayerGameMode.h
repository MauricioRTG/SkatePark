// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SkateParkMultiplayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SKATEPARK_API ASkateParkMultiplayerGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void PointsAcquired(class ASkateCharacterController* PlayerController, int32 ScoreAmount);

	UPROPERTY(VisibleAnywhere)
	class ASkateParkGameState* SkateParkGameState;

	virtual void BeginPlay() override;
};
