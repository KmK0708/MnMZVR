// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidjet.generated.h"

UCLASS()
class MNMZVR_API UMainWidjet : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySetting|Score", meta = (BindWidget))
	class UTextBlock* Title;
};