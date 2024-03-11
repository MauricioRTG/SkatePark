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
	uint32 SecondsLeftInMatch = FMath::CeilToInt(MatchTime - GetWorld()->GetTimeSeconds());
	//Checks if time has passed
	if (Countdown != SecondsLeftInMatch)
	{
		SetMinutesAndSeconds(MatchTime - GetWorld()->GetTimeSeconds());
	}
	//Store seconds left in match to commpare if time has passed
	Countdown = SecondsLeftInMatch;
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

