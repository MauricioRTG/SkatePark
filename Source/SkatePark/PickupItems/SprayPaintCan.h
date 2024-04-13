// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItem.h"
#include "SprayPaintCan.generated.h"

/**
 * 
 */
UCLASS()
class SKATEPARK_API ASprayPaintCan : public APickupItem
{
	GENERATED_BODY()

public:
	ASprayPaintCan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool FromSweep,
		const FHitResult& SweepResult
	) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowprivateAccess = "true"));
	class ASkateParkMultiplayerGameMode* GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"));
	int32 PointsAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowprivateAccess = "true"));
	class USoundBase* Sound;
};
