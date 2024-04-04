// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SkateParkGameState.generated.h"

/**
 * 
 */
UCLASS()
class SKATEPARK_API ASkateParkGameState : public AGameState
{
	GENERATED_BODY()

public:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void UpdateTopScore(class ASkatePlayerState* SkatePlayerState);

	UPROPERTY(VisibleAnywhere,Replicated)
	TArray<ASkatePlayerState*> TopScoringPlayers;

private:

	int32 TopScore = 0;
};
