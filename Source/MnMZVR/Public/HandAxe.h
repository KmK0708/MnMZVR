// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeaponBase.h"
#include "HandAxe.generated.h"

/**
 * 
 */
UCLASS()
class MNMZVR_API AHandAxe : public AMeleeWeaponBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AHandAxe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
