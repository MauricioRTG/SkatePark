// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpOverCheckpoints.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "SkatePark/PlayerCharacter/SkateParkCharacter.h"
#include "SkatePark/PlayerState/SkatePlayerState.h"
#include "SkatePark/PlayerCharacter/SkateCharacterController.h"
#include "SkatePark/GameMode/SkateParkMultiplayerGameMode.h"

// Sets default values
AJumpOverCheckpoints::AJumpOverCheckpoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Checkpoint replicates to clients
	bReplicates = true;

	/* Arrow*/
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	/*Collision box*/
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Arrow);
	//Set Collision to no collide whith anything
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*Static Mesh*/
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionBox);

	//Set points amount to grant to player
	PointsAmount = 10;
	MinDotProductParallelThreshold = 200.f;
	RespawnDelay = 10.f;
	
}

// Called every frame
void AJumpOverCheckpoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AJumpOverCheckpoints::BeginPlay()
{
	Super::BeginPlay();

	//Get GameMode
	UWorld* World = GetWorld();
	if (World)
	{
		GameMode = Cast<ASkateParkMultiplayerGameMode>(World->GetAuthGameMode());
		if (!GameMode)
		{
			UE_LOG(LogTemp, Log, TEXT("GameMode not found in JumpOverCheckpoints class"));
		}
	}

	/*Collision Handling*/
	//Allow trigger overlap envents
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Only overlap with player
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//Bind callback to delegate
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AJumpOverCheckpoints::OnBoxEndOverlap);
	
}

void AJumpOverCheckpoints::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ASkateParkCharacter* Character = Cast<ASkateParkCharacter>(OtherActor);
	if (Character)
	{
		UE_LOG(LogTemp, Log, TEXT("OverlapEnd"));
		//Get Arrow rotation in world space
		FRotator ArrowRotation = Arrow->GetComponentRotation();
		//Get Arrow x rotation vector
		FVector VectorRotation(ArrowRotation.Vector());

		//If is equal to zero it means that the vector are perpendicular or oposite from each other
		double DotProduct = FVector::DotProduct(VectorRotation, Character->GetVelocity());
		UE_LOG(LogTemp, Log, TEXT("DotProduct: %f"), DotProduct);
		
		//Vector are nearly parallel
		if (DotProduct > MinDotProductParallelThreshold)
		{
			//PlaySound
			if (Sound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
			}

			//If player traverse the box in the correct direction update points
			ASkateCharacterController* PlayerController = Cast<ASkateCharacterController>(Character->GetController());
			if (PlayerController)
			{
				if (GameMode)
				{
					GameMode->PointsAcquired(PlayerController, PointsAmount);
				}
			}

			//Hide Actor and start timer to respawn it again
			StartTimer();
		}
	}
}

void AJumpOverCheckpoints::RespawnDelayFunction()
{
	//Unhidde actor and clears timer
	SetActorHiddenInGame(false);
	//Allow trigger overlap envents
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Only overlap with player
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AJumpOverCheckpoints::StartTimer()
{
	//Hidde actor in game
	SetActorHiddenInGame(true);

	//Ignore collisions
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Do not trigger ovelap events yet
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Delay respawn
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AJumpOverCheckpoints::RespawnDelayFunction, RespawnDelay, false);
}



