// Fill out your copyright notice in the Description page of Project Settings.


#include "MnMZGameModeBase.h"
#include "MainWidjet.h"
void AMnMZGameModeBase::BeginPlay()
{
	// ���� �������Ʈ�� �����Ѵ�.
	UMainWidjet* main_UI = CreateWidget<UMainWidjet>(GetWorld(), mainWidget);

	if (main_UI != nullptr)
	{
		// ������ ������ �� ��Ʈ�� �׸���.
		main_UI->AddToViewport();
	}
	
	
}
