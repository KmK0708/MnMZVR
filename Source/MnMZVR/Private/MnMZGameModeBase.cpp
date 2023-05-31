// Fill out your copyright notice in the Description page of Project Settings.


#include "MnMZGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayer.h"
#include "MainUI.h"
#include "Components/WidgetComponent.h"



AMnMZGameModeBase::AMnMZGameModeBase()
{
}

void AMnMZGameModeBase::BeginPlay()
{
}

void AMnMZGameModeBase::ShowVictoryWidget()
{
// 	FInputModeUIOnly inputMode;
// 	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
// 	GetWorld()->GetFirstPlayerController()->SetInputMode(inputMode);
// 
// 	winEnd_UI = CreateWidget<UUserWidget>(GetWorld(), endWinWidget);
// 
// 	if (winEnd_UI != nullptr)
// 	{
// 		winEnd_UI->AddToViewport(100);
// 	}
// 
// 	FTimerHandle endTimerHandle;
// 	GetWorldTimerManager().SetTimer(endTimerHandle, this, &AKillingFloorGameModeBase::OpenStartLevel, 5.0f);
}

void AMnMZGameModeBase::ShowDefeatWidget()
{
}

void AMnMZGameModeBase::ShowPlayerHitWidget()
{
}

void AMnMZGameModeBase::RemovePlayerHitWidget()
{
}
