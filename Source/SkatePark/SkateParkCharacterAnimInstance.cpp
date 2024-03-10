// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateParkCharacterAnimInstance.h"
#include "SkateParkCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void USkateParkCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ASkateParkCharacter>(TryGetPawnOwner());
}

void USkateParkCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (Character == nullptr)
	{
		Character = Cast<ASkateParkCharacter>(TryGetPawnOwner());
	}

	if (Character == nullptr)
	{
		return;
	}

	//Get Velocity and set speed
	FVector Velocity = Character->GetVelocity();
	Speed = Velocity.Size();

	//Set true if character is in air
	bIsInAir = Character->GetCharacterMovement()->IsFalling();

	//Set true if character is accelerating
	bIsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
}

