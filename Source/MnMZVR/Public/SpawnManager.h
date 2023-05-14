// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

UCLASS()
class MNMZVR_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		// 자리가 비어있는지 bool 배열
	UPROPERTY()
	TArray<bool> bIsWaitzoneClear;

	// 구매했는지 bool 배열
	UPROPERTY()
	TArray<bool> bIsBuy;

	// 플레이어가 있는지 여부
	UPROPERTY()
	TArray<bool> bIsPlayer;

	bool bCheckWait;

	bool bCheckSpawn;

	UPROPERTY()
	TArray<int32> orderItemIdx = {0, 0, 0};

	int32 CheckCustomerNum;
	
	UPROPERTY()
	int32 CheckMeshCount = 3;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACustomerCharacter> CustomerFactory;

	void CheckWaitzone();

	UFUNCTION()
	void SpawnCustomer();
	
	void SpawnCustom();

	//마석
	TArray<FString> hearthstone = { "hearthstone" };
	//철광석
	TArray<FString> ironore = { "iron ore" };
	//물약
	TArray<FString> StrangePotion = { "StrangePotion" };
	//금붙이
	TArray<FString> SuspiciousMetal = { "SuspiciousMetal" };

};
