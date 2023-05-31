// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainPlayer.h"
#include "MnMZGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MNMZVR_API AMnMZGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
AMnMZGameModeBase();

virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	class UMainUI* MainUI;

	// Player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMainPlayer* MainPlayer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> EndWinWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> EndLoseWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> PlayerHitWidget;


	void ShowVictoryWidget();
	void ShowDefeatWidget();
	void ShowPlayerHitWidget();
	void RemovePlayerHitWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget* winEnd_UI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget* loseEnd_UI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget* playerHit_UI;
};
