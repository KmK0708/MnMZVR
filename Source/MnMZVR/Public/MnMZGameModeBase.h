// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MnMZGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MNMZVR_API AMnMZGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category=MyDefaultSetting)
	TSubclassOf<class UMainWidjet> mainWidget;

	virtual void BeginPlay() override;
	
};
