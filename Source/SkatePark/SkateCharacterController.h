// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SkateCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class SKATEPARK_API ASkateCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	ASkateCharacterController();

	void SetMinutesAndSeconds(float CountdownTime);

	FORCEINLINE int32 GetMinutes() { return Minutes; };

	FORCEINLINE	int32 GetSeconds() { return Seconds; };

	virtual void Tick(float DeltaTime) override;

	void GameOver();

protected:
	virtual void BeginPlay() override;

	void SetTimer();

private:
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (AllowPrivateAccess = "true"))
	int32 Seconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (AllowPrivateAccess = "true"))
	int32 Minutes;

	//Match time in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (AllowPrivateAccess = "true"))
	float MatchTime = 120.f;

	uint32 Countdown = 0;

	bool bStartTimer = true;
};
