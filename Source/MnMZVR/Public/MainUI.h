// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class MNMZVR_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInfo", meta = (BindWidget))
	class UProgressBar* PlayerHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInfo", meta = (BindWidget))
	class UTextBlock* playerMoney;

	virtual void NativeConstruct() override;

	UPROPERTY()
	class AMnMZGameModeBase* GM;
	UPROPERTY()
	class AMainPlayer* Mainplayer;

	void PrintRemainingTime();
	void PrintPlayerHP();
	UFUNCTION(BlueprintCallable)
	void PrintPlayerMoney();
};
