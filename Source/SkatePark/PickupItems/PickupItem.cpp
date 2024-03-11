// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

// Sets default values
APickupItem::APickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Allows replication of Actor to clients
	bReplicates = true;

	/*PickupItem Mesh*/
	PickupItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PickupItemMesh"));
	SetRootComponent(PickupItemMesh);
	//When dropped it need to collide with all the objects in scene
	PickupItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//The player should not collide with the pickup item
	PickupItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//But for now we don't want that behavior so we set to not collide, we change later this
	PickupItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*Area Sphere*/
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	//Ignore collisions
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Do not trigger ovelap events yet
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RespawnDelay = 10.f;
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	/*Collision Handling*/
	//Allow trigger overlap envents
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Only overlap with player
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	/*Ovelap events*/
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnSphereOverlap);
	
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
}

void APickupItem::RespawnDelayFunction()
{
	//Unhidde actor and clears timer
	SetActorHiddenInGame(false);

	//Allow trigger overlap envents
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Only overlap with player
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void APickupItem::StartTimer()
{
	//Hidde actor in game
	SetActorHiddenInGame(true);

	//Ignore collisions
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Do not trigger ovelap events yet
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Delay respawn
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APickupItem::RespawnDelayFunction, RespawnDelay, false);
}




