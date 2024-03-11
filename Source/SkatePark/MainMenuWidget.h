// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKATEPARK_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;

	UFUNCTION()
	void OnStartGameButtonClicked();
};
