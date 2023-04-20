// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeaponBase.h"
#include "LongSword.generated.h"

/**
 * 
 */
UCLASS()
class MNMZVR_API ALongSword : public AMeleeWeaponBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ALongSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
