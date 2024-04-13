// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateCharacterController.h"
#include "Engine/EngineTypes.h"
#include "SkateParkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SkatePark/GameStates/SkateParkGameState.h"
#include "SkatePark/PlayerState/SkatePlayerState.h"
#include "GameFramework/PlayerState.h"

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

	/*
	* Using MultiCast RPC from Server
	* Pros: Centralized control on the server, ensuring consistent behavior across all clients. Efficient when a single action affects all clients.
	* Cons: Requires managing RPC calls and ensuring proper network replication. Might introduce overhead if frequent updates are needed.
	* When to Choose: the action of hiding widgets needs to be synchronized across all clients (i.e., all clients should see the same widgets hidden simultaneously)
	*/

	/*if (IsLocalController())
	{
		//Hide character overlay and show game over widget
		ServerRequestHideCharacterOverlay();
	}*/

	/*
	* Using Iterating Over PlayerArray of the GameState on each client
	* Pros: Straightforward and doesn't require additional RPC calls. Each client handles widget visibility locally.
	* Cons: May result in increased network traffic if many clients are updating frequently. Might lead to slight discrepancies if updates are not synchronized perfectly.
	* Choosed because: widget hiding is purely local and doesn't affect other clients' gameplay (e.g., hiding UI elements specific to each player's view)
	*/
	
	UWorld* World = GetWorld();
	if (World)
	{
		//Hide character overlay widgets of all clients and server by iterating PlayerArray from GameState
		//We could also use a Multicast RPC to hide the widgets
		if (IsLocalController())
		{
			ASkateParkGameState* SkateParkGameState = Cast<ASkateParkGameState>(World->GetGameState());
			if (SkateParkGameState)
			{
				for (APlayerState* CurrentPlayerState : SkateParkGameState->PlayerArray)
				{
					ASkatePlayerState* SkatePlayerState = Cast<ASkatePlayerState>(CurrentPlayerState);
					if (SkatePlayerState)
					{
						ASkateParkCharacter* PlayerCharacter = Cast<ASkateParkCharacter>(SkatePlayerState->GetPawn());
						if (PlayerCharacter)
						{
							//Remove widgets from viewport
							PlayerCharacter->HideTimer();
							PlayerCharacter->HidePoints();
						}
					}
				}
			}
		}

		//Show GameOver
		ASkateParkCharacter* SkatePlayerCharacter = Cast<ASkateParkCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		if (SkatePlayerCharacter)
		{
			//Create EndGame widget
			SkatePlayerCharacter->ShowGameOver();
		}
	}
}

void ASkateCharacterController::ServerRequestHideCharacterOverlay_Implementation()
{
	MulticastRequestHideCharacterOverlay();
}

void ASkateCharacterController::MulticastRequestHideCharacterOverlay_Implementation()
{
	/*Code to hide widgets*/
}

void ASkateCharacterController::SetStartTimer(bool StartTimer)
{
	bStartTimer = StartTimer;
}



