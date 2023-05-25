// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeaponBase.h"
#include "Mace.generated.h"

/**
 * 
 */
UCLASS()
class MNMZVR_API AMace : public AMeleeWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
