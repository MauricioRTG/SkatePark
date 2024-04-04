// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "SkatePark/GameStates/SkateParkGameState.h"
#include "SkatePark/PlayerState/SkatePlayerState.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RestartGameButton)
	{
		RestartGameButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::OnRestartGameButtonClicked);
	}

	LevelName = "MainMenuLevel";
}

bool UGameOverWidget::Initialize()
{
	if (!Super::Initialize()) return false;

	if (TopScorerTextField)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			TopScorerTextField->TextDelegate.BindUFunction(this, FName("SetTopScorerTextField"));
		}
	}

	return true;
}

void UGameOverWidget::OnRestartGameButtonClicked()
{
	//Get current level name
	//FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	//Restart level
	UGameplayStatics::OpenLevel(GetWorld(), FName(LevelName));
}

FText UGameOverWidget::SetTopScorerTextField()
{

	UWorld* World = GetWorld();
	if (World)
	{
		ASkateParkGameState* GameState = Cast<ASkateParkGameState>(UGameplayStatics::GetGameState(World));
		ASkatePlayerState* PlayerState = Cast<ASkatePlayerState>(UGameplayStatics::GetPlayerState(World, 0));
		if (GameState && PlayerState)
		{
			TArray<ASkatePlayerState*> TopPlayers = GameState->TopScoringPlayers;
			FString TopPlayersInfo;

			if (TopPlayers.Num() == 0)
			{
				TopPlayersInfo = FString("There is no winner");
			}
			else if (TopPlayers.Num() == 1 && TopPlayers[0] == PlayerState)
			{
				TopPlayersInfo = FString("You are the winner!!");
			}
			else if (TopPlayers.Num() == 1)
			{
				TopPlayersInfo = FString::Printf(TEXT("Winner: %s"), *TopPlayers[0]->GetPlayerName());
			}
			else if (TopPlayers.Num() > 1)
			{
				TopPlayersInfo = FString("Players tied:\n");
				for (auto TiedPlayer : TopPlayers)
				{
					TopPlayersInfo.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
				}
			}

			return FText::FromString(TopPlayersInfo);
		}

		return FText::FromString("NULL");
	}

	return FText::FromString("NULL");
}
