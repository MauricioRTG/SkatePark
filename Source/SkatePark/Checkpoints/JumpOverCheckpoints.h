// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpOverCheckpoints.generated.h"

UCLASS()
class SKATEPARK_API AJumpOverCheckpoints : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpOverCheckpoints();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*Arrow Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* Arrow;

	/*Collision box*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* CollisionBox;

	/*Static Mesh Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

	/*Unhidde actor and clears timer */
	void RespawnDelayFunction();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	class ASkateParkMultiplayerGameMode* GameMode;

	void StartTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Callback to bind to OnComponentEndOverlap delegate
	UFUNCTION()
		void OnBoxEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex
		);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CollisionDetection, meta = (AllowPrivateAccess = "true"))
	int32 PointsAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowprivateAccess = "true"));
	class USoundBase* Sound;

	//Min threshold to consider that two vector are parallel
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CollisionDetection, meta = (AllowPrivateAccess = "true"))
	float MinDotProductParallelThreshold;

	//Delay for respawning checkpoint after player overlapped with it
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CollisionDetection, meta = (AllowPrivateAccess = "true"))
	float RespawnDelay;

	FTimerHandle TimerHandle;
};
