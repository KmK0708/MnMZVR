// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "MnMZGameModeBase.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayer.h"
#include "Components/Image.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	GM = Cast<AMnMZGameModeBase>(GetWorld()->GetAuthGameMode());

	Mainplayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
// 	if (GM->main_UI != nullptr)
// 	{
// 		//gm->main_UI->PrintCurrentEnemyLeft();
// 		GM->main_UI->PrintRemainingTime();
// 		GM->main_UI->PrintCurrentWave();
// 		GM->main_UI->PrintLastWave();
// 		GM->main_UI->PrintPlayerHP();
// 		GM->main_UI->PrintPlayerMoney();
// 		GM->main_UI->PrintWaveImage(Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Script/Engine.Texture2D'/Game/Assets/UI/wall-clock.wall-clock'"))));
// 	}	
}

void UMainUI::PrintRemainingTime()
{
}

void UMainUI::PrintPlayerHP()
{
	if (Mainplayer != nullptr && GM != nullptr)
	{
		// 프로그래스바
		//float hp = Mainplayer->GetHP();
		//float maxHP = Mainplayer->GetMaxHP();
	}
}

void UMainUI::PrintPlayerMoney()
{
	if (Mainplayer != nullptr && GM != nullptr)
	{
		FText Money = FText::AsNumber(Mainplayer->PlayerMoney);
		playerMoney->SetText(Money);
	}
}
