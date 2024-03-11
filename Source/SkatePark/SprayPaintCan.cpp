// Fill out your copyright notice in the Description page of Project Settings.


#include "SprayPaintCan.h"
#include "SkateParkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

ASprayPaintCan::ASprayPaintCan()
{
	PointsAmount = 10;
}

void ASprayPaintCan::BeginPlay()
{
	Super::BeginPlay();
}

void ASprayPaintCan::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, FromSweep, SweepResult);

	//When Character overlaps spray paint can update the player's points and destroy the pickup item
	ASkateParkCharacter* SkateParkCharacter = Cast<ASkateParkCharacter>(OtherActor);
	if (SkateParkCharacter)
	{
		//PlaySound
		if (Sound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
		}

		SkateParkCharacter->UpdatePoints(PointsAmount);
		StartTimer();
	}
}
