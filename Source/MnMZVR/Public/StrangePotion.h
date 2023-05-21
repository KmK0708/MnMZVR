// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DropItemBase.h"
#include "StrangePotion.generated.h"

/**
 * 
 */
UCLASS()
class MNMZVR_API AStrangePotion : public ADropItemBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AStrangePotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
