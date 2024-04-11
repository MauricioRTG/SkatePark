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

	void SetStartTimer(bool StartTimer);

	virtual void Tick(float DeltaTime) override;

	void GameOver();

	virtual float GetServerTime(); // Synced with server world clock
	virtual void ReceivedPlayer() override; // Sync with server clock as soon as possible

protected:
	virtual void BeginPlay() override;

	void SetTimer();

	/* Sync Time between client and server*/
	// Requests the current server time, passing in the client's time when the request was sent
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequestTime);

	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
	void ClientRequestClientTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	//Difference between current server time and current client time
	float ClientServerDelta = 0.f;

	float TimeSyncRunningTime = 0.f;
	void CheckTimeSync(float DeltaTime);

	//Sync time frecuency
	float TimeSyncRequest = 5.f;

	/*HUD*/

	UFUNCTION(Server, Reliable)
	void ServerRequestHideCharacterOverlay();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRequestHideCharacterOverlay();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timer", meta = (AllowPrivateAccess = "true"))
	bool bStartTimer = true;
};
