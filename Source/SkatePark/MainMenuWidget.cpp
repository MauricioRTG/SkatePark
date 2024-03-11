// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SkateParkCharacter.h"
#include "SkateCharacterController.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartGameButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnStartGameButtonClicked);
}

void UMainMenuWidget::OnStartGameButtonClicked()
{
	//Get player controller reference
	ASkateCharacterController* PlayerController = Cast<ASkateCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//Interact with game elements only, and don't show cursor, also start timer
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetStartTimer(true);
	}

	//Get PlayerCharacter reference
	ASkateParkCharacter* PlayerCharacter = Cast<ASkateParkCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		//Show HUD widgets
		PlayerCharacter->ShowTimer();
		PlayerCharacter->ShowPoints();
	}

	//Remove widget from viewport
	RemoveFromParent();

	//Destruct instance
	Destruct();
}

