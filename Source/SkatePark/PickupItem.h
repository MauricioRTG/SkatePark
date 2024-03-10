// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UCLASS()
class SKATEPARK_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APickupItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*Unhidde actor and clears timer */
	virtual void RespawnDelayFunction();

	//Hidde actor and start timer
	virtual void StartTimer();

	FTimerHandle TimerHandle;

	//Delay for respawning checkpoint after player overlapped with it
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CollisionDetection, meta = (AllowPrivateAccess = "true"))
	float RespawnDelay;

	//Callback to bind to OnComponentBeginOverlap delegate
	UFUNCTION()
		virtual void OnSphereOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex,
			bool FromSweep,
			const FHitResult& SweepResult
		);

	UPROPERTY(VisibleAnywhere, Category = "PickupItem Properties")
		class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		USkeletalMeshComponent* PickupItemMesh;

private:
	
};
