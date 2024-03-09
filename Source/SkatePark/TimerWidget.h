// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKATEPARK_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MinutesTextField;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SecondsTextField;

	UFUNCTION(BlueprintCallable)
	FText SetMinutesTextField();

	UFUNCTION(BlueprintCallable)
	FText SetSecondsTextField();
};
