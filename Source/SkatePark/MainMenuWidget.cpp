// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SkateParkCharacter.h"
#include "SkateCharacterController.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartGameButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnStartGameButtonClicked);
	LevelName = "FantasyLevel";

	QuitGameButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnQuitGameButtonClicked);

	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//Interact with game elements only, and don't show cursor, also start timer
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
	}
}

void UMainMenuWidget::OnStartGameButtonClicked()
{
	//Travel to Main level
	UGameplayStatics::OpenLevel(GetWorld(), FName(LevelName));
}

void UMainMenuWidget::OnQuitGameButtonClicked()
{
	//Quit Game
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

