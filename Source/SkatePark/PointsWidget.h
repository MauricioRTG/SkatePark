// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PointsWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKATEPARK_API UPointsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	bool Initialize() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PointsTextField;

	UFUNCTION(BlueprintCallable)
	FText SetPointsTextField();
};
