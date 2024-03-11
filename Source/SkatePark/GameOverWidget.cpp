// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RestartGameButton)
	{
		RestartGameButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::OnRestartGameButtonClicked);
	}
}

void UGameOverWidget::OnRestartGameButtonClicked()
{
	//Get current level name
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	//Restart level
	UGameplayStatics::OpenLevel(GetWorld(), FName(CurrentLevelName));
}