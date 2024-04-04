// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKATEPARK_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;
	bool Initialize() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartGameButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TopScorerTextField;

	UFUNCTION()
	void OnRestartGameButtonClicked();

	UFUNCTION()
	FText SetTopScorerTextField();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	FString LevelName;
};
