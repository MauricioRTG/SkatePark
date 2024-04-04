// Fill out your copyright notice in the Description page of Project Settings.


#include "SprayPaintCan.h"
#include "SkatePark/PlayerCharacter/SkateParkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "SkatePark/PlayerState/SkatePlayerState.h"
#include "SkatePark/PlayerCharacter/SkateCharacterController.h"
#include "SkatePark/GameMode/SkateParkMultiplayerGameMode.h"

ASprayPaintCan::ASprayPaintCan()
{
	PointsAmount = 10;
}

void ASprayPaintCan::BeginPlay()
{
	Super::BeginPlay();

	//Get GameMode
	UWorld* World = GetWorld();
	if (World)
	{
		GameMode = Cast<ASkateParkMultiplayerGameMode>(World->GetAuthGameMode());
		if (!GameMode)
		{
			UE_LOG(LogTemp, Log, TEXT("GameMode not found in SpratPainCan class"));
		}
	}
}

void ASprayPaintCan::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, FromSweep, SweepResult);

	//When Character overlaps spray paint can update the player's points and hide the pickup item
	ASkateParkCharacter* SkateParkCharacter = Cast<ASkateParkCharacter>(OtherActor);
	if (SkateParkCharacter)
	{
		//PlaySound
		if (Sound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
		}

		//Update Score
		ASkateCharacterController* PlayerController = Cast<ASkateCharacterController>(SkateParkCharacter->GetController());
		if (PlayerController)
		{
			GameMode->PointsAcquired(PlayerController, PointsAmount);
		}

		//Hide pickup item and start timer to undhide after a time
		StartTimer();
	}
}
