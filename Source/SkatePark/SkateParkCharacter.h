// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SkateParkCharacter.generated.h"


UCLASS(config=Game)
class ASkateParkCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkateBoard;

public:
	ASkateParkCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/* Returns Player Score*/
	FORCEINLINE int32 GetPlayerPoints() const { return Points; }

	UFUNCTION(BlueprintCallable)
	void UpdatePoints(int32 PointsAmount);

	virtual void Tick(float DeltaTime) override;

	/* UI */
	//Create Main menu widget instance on screen
	void ShowMainMenu();

	//Create points widget instance on screen
	void ShowPoints();
	void HidePoints();

	//Create timer widget instance on screen
	void ShowTimer();
	void HideTimer();

	//Create Game Over widget instance on screen
	void ShowGameOver();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void SlowDown(float Deltatime);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();


private:

	APlayerController* PlayerController;

	/*Player's score updated with each coin collected*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Points, meta = (AllowPrivateAccess = "true"))
	int32 Points;

	/*Movement*/

	//Speed when moving backwards
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BackwardsMaxWalkSpeed;

	//Speed when moving forward
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float NormalMaxWalkSpeed;

	//Lerp alpha (interpolation speed) when the skateboard starts speeding up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SpeedUpLerpMovementAlpha;

	//Interp speed when skate starts to slow down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlowDownInterpSpeed;

	//Movement forward interpolation for speeding up and slowing down
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float InterpMovementVectorY;

	//Min threshold for Movement forward interpolation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MinInterpMovementVectorY;

	//Turn rate when moving 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnRate;

	/*UI Classes and Instances*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMainMenuWidget> MainMenuClass;

	UMainMenuWidget* MainManuInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPointsWidget> PointsClass;

	UPointsWidget* PointsInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTimerWidget> TimerClass;

	UTimerWidget* TimerInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameOverWidget> GameOverClass;

	UGameOverWidget* GameOverInstance;

};

