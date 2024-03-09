// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerWidget.h"
#include "Components/TextBlock.h"
#include "Engine.h"
#include "Internationalization/Text.h"
#include "SkateParkGameMode.h"
#include "SkateCharacterController.h"

bool UTimerWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) { return false; }

	//Checks if both seconds and minutes text fields exist
	if (SecondsTextField && MinutesTextField)
	{
		//Check if world context exists
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			SecondsTextField->TextDelegate.BindUFunction(this, "SetSecondsTextField");
			MinutesTextField->TextDelegate.BindUFunction(this, "SetMinutesTextField");
		}
	}

	return true;
}

FText UTimerWidget::SetMinutesTextField()
{
	UWorld* World = GetWorld();

	//Check if world exits
	if (World != nullptr)
	{

		ASkateCharacterController* CharacterController = Cast<ASkateCharacterController>(World->GetFirstPlayerController());
		//Check if game mode exits
		if (CharacterController)
		{
			//Return Minutes value from game mode with a minimum and maximum of 2 integral digits 00
			FNumberFormattingOptions FormattingOptions;
			FormattingOptions.MinimumIntegralDigits = 2;
			FormattingOptions.MaximumIntegralDigits = 2;
			FormattingOptions.MaximumFractionalDigits = 0;

			//Convert Int32 to FText
			FText MinutesText = FText::AsNumber(CharacterController->GetMinutes(), &FormattingOptions);

			return MinutesText;
		}
		else
		{
			//Return Null
			return FText::FromString("Null");
		}
	}
	return FText::FromString("Null");
}

FText UTimerWidget::SetSecondsTextField()
{
	UWorld* World = GetWorld();

	//Check if world exits
	if (World != nullptr)
	{
		ASkateCharacterController* CharacterController = Cast<ASkateCharacterController>(World->GetFirstPlayerController());

		//Check if game mode exits
		if (CharacterController)
		{
			//Return Minutes value from game mode with a minimum and maximum of 2 integral digits 00
			FNumberFormattingOptions FormattingOptions;
			FormattingOptions.MinimumIntegralDigits = 2;
			FormattingOptions.MaximumIntegralDigits = 2;
			FormattingOptions.MaximumFractionalDigits = 0;

			//Convert Int32 to FText
			FText SecondsText = FText::AsNumber(CharacterController->GetSeconds(), &FormattingOptions);
			return SecondsText;
		}
		else
		{
			//Return Null
			return FText::FromString("Null");
		}
	}
	return FText::FromString("Null");
}
