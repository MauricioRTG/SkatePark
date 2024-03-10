// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateParkCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SkeletalMeshComponent.h"
#include "PointsWidget.h"
#include "TimerWidget.h"


//////////////////////////////////////////////////////////////////////////
// ASkateParkCharacter

ASkateParkCharacter::ASkateParkCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	TurnRate = 0.1f;
	LerpMovementAlpha = 0.01f;
	LerpMovementVectorY = 0.f;
	BackwardsMaxWalkSpeed = 50.f;
	NormalMaxWalkSpeed = 500.f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = NormalMaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Skateboard skeletal mesh
	SkateBoard = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkateBoard"));
	SkateBoard->SetupAttachment(RootComponent);

	//Set points
	Points = 0;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ASkateParkCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//UI elements
	ShowPoints();
	ShowTimer();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASkateParkCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateParkCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASkateParkCharacter::Look);

	}
}

void ASkateParkCharacter::UpdatePoints(int32 PointsAmount)
{
	Points = Points + PointsAmount;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Points);
}

void ASkateParkCharacter::ShowPoints()
{
	if (PointsClass)
	{
		PointsInstance = CreateWidget<UPointsWidget>(GetWorld(), PointsClass);
		if (PointsInstance)
		{
			PointsInstance->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Points Widget Instance not created"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Points Widget Class not set"));
	}
}


void ASkateParkCharacter::ShowTimer()
{
	if (TimerClass)
	{
		TimerInstance = CreateWidget<UTimerWidget>(GetWorld(), TimerClass);
		if (TimerInstance)
		{
			TimerInstance->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Timer Widget Instance not created"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Timer Widget Class not set"));
	}
}

void ASkateParkCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//When character moves backwards don't orient rotation to movement and reduce the MaxWalkSpeed
		if (MovementVector.Y < 0)
		{
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->MaxWalkSpeed = BackwardsMaxWalkSpeed;
			//The right vector in the SKM_Skateboard points to the front of the skateboard, so add movement to go backwards
			AddMovementInput(SkateBoard->GetRightVector(), MovementVector.Y);
		}
		else //Else orient rotation to Movement and to return to NormalMaxWalkSpeed
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->MaxWalkSpeed = NormalMaxWalkSpeed;

			/*Add Movement*/
			//Lerp MovementVector.Y so the character smoothly slows down
			LerpMovementVectorY = FMath::Lerp(MovementVector.Y, LerpMovementVectorY, LerpMovementAlpha);
			UE_LOG(LogTemp, Warning, TEXT("LerpMovementVectorY : %f"), LerpMovementVectorY);

			//The right vector in the SKM_Skateboard points to the front of the skateboard, so add movement to go forward
			AddMovementInput(SkateBoard->GetRightVector(), LerpMovementVectorY);

			//The foward vector in the SKM_Skateboard points to left so we multiply by one so is the correct right vector of the skateboard and go left or right
			//Also multiplay for turn rate for desired turn rotation speed
			FVector RightVector = (SkateBoard->GetForwardVector() * -1.f) * TurnRate;

			AddMovementInput(RightVector, MovementVector.X);
		}
	}
}

void ASkateParkCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




