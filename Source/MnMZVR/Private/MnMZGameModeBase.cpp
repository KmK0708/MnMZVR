// Fill out your copyright notice in the Description page of Project Settings.


#include "MnMZGameModeBase.h"
#include "MainWidjet.h"
void AMnMZGameModeBase::BeginPlay()
{
	// 위젯 블루프린트를 생성한다.
	UMainWidjet* main_UI = CreateWidget<UMainWidjet>(GetWorld(), mainWidget);

	if (main_UI != nullptr)
	{
		// 생성된 위젯을 뷰 포트에 그린다.
		main_UI->AddToViewport();
	}
	
	
}
