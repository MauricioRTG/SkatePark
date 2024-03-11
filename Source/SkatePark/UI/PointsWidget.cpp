// Fill out your copyright notice in the Description page of Project Settings.


#include "PointsWidget.h"
#include "Components/TextBlock.h"
#include "SkatePark/PlayerCharacter/SkateParkCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SkatePark/SkateParkGameMode.h"

bool UPointsWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	if (PointsTextField)
	{
		//Check if world context exists
		UWorld* World = GetWorld();
		if (World)
		{
			//Binds callback to delegate
			PointsTextField->TextDelegate.BindUFunction(this, "SetPointsTextField");
		}
	}

	return true;
}

FText UPointsWidget::SetPointsTextField()
{
	UWorld* World = GetWorld();

	if (World)
	{
		ASkateParkCharacter* SkateParkCharacter = Cast<ASkateParkCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		if (SkateParkCharacter)
		{
			//Return player score from player character
			return FText::FromString(FString::FromInt(SkateParkCharacter->GetPlayerPoints()));
		}
		else
		{
			return FText::FromString("NULL");
		}
	}

	return FText::FromString("NULL");
}