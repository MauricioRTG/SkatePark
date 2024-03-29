// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateCharacterController.h"
#include "Engine/EngineTypes.h"
#include "SkateParkCharacter.h"
#include "Kismet/GameplayStatics.h"

ASkateCharacterController::ASkateCharacterController()
{
	//Set Minutes and Seconds for timer countdown
	Seconds = 0;
	Minutes = 0;
}

void ASkateCharacterController::BeginPlay()
{
	Super::BeginPlay();
}

void ASkateCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStartTimer)
	{
		SetTimer();
	}

	//Checks if time needs to sync
	CheckTimeSync(DeltaTime);
}

void ASkateCharacterController::SetMinutesAndSeconds(float CountdownTime)
{
	//Pass seconds to minutes
	Minutes = FMath::FloorToInt(CountdownTime / 60.f);
	//Seconds left in that minute
	Seconds = CountdownTime - Minutes * 60;

	//Time is up
	if (Minutes == 0 && Seconds == 0)
	{
		GameOver();
	}

	UE_LOG(LogTemp, Log, TEXT("%i : %i"), Minutes, Seconds);
}

void ASkateCharacterController::SetTimer()
{
	//Seconds left in match
	uint32 SecondsLeftInMatch = FMath::CeilToInt(MatchTime - GetServerTime());
	//Checks if time has passed
	if (Countdown != SecondsLeftInMatch)
	{
		SetMinutesAndSeconds(MatchTime - GetServerTime());
	}
	//Store seconds left in match to commpare if time has passed
	Countdown = SecondsLeftInMatch;
}

//RPC from client to server
void ASkateCharacterController::ServerRequestServerTime_Implementation(float TimeOfClientRequestTime)
{
	//Get Current server time without considering message travel time to client from server
	float ServerTimeReceipt = GetWorld()->GetTimeSeconds();
	ClientRequestClientTime(TimeOfClientRequestTime, ServerTimeReceipt);
}

//RPC from server to client
void ASkateCharacterController::ClientRequestClientTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	//The time it takes for data to be sent across the network to the server from the client and back to the client from the server
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	//Get Current server time without considering message travel time to client from server
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);

	//Difference between current server time and current client time
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void ASkateCharacterController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	//Is not a simulated proxy and only update if if the specified sync request time have passed
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncRequest)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

float ASkateCharacterController::GetServerTime()
{
	//If server
	if (HasAuthority())
	{
		return GetWorld()->GetTimeSeconds();
	}
	else
	{
		//If client take into account client-server delta
		return GetWorld()->GetTimeSeconds() + ClientServerDelta;
	}
}

void ASkateCharacterController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	// Sync with server clock as soon as possible
	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ASkateCharacterController::GameOver()
{
	bStartTimer = false;

	ASkateParkCharacter* PlayerCharacter = Cast<ASkateParkCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		//Remove widgets from viewport
		PlayerCharacter->HideTimer();
		PlayerCharacter->HidePoints();

		//Create EndGame widget
		PlayerCharacter->ShowGameOver();
	}
}

void ASkateCharacterController::SetStartTimer(bool StartTimer)
{
	bStartTimer = StartTimer;
}



