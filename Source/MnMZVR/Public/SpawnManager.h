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

		// �ڸ��� ����ִ��� bool �迭
	UPROPERTY()
	TArray<bool> bIsWaitzoneClear;

	// �����ߴ��� bool �迭
	UPROPERTY()
	TArray<bool> bIsBuy;

	// �÷��̾ �ִ��� ����
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

	//����
	TArray<FString> hearthstone = { "hearthstone" };
	//ö����
	TArray<FString> ironore = { "iron ore" };
	//����
	TArray<FString> StrangePotion = { "StrangePotion" };
	//�ݺ���
	TArray<FString> SuspiciousMetal = { "SuspiciousMetal" };

};
