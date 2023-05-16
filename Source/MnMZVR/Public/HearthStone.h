// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DropItemBase.h"
#include "HearthStone.generated.h"

/**
 * 
 */
UCLASS()
class MNMZVR_API AHearthStone : public ADropItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHearthStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
