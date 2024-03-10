// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SkateParkCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SKATEPARK_API USkateParkCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaTime) override;

private:

	UPROPERTY(BlueprintReadOnly, Category = "Character Movement", meta = (AllowPrivateAccess = "true"))
	class ASkateParkCharacter* Character;

	UPROPERTY(BlueprintReadOnly, Category = "Character Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Character Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Character Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
};
