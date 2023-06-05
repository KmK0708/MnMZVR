// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropItemBase.generated.h"

UCLASS()
class MNMZVR_API ADropItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UPrimitiveComponent* rootComp;

	// ������ �޽�
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ItemMesh;
	// �տ� ��Ҵ��� ���Ǿ��ݸ���
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USphereComponent* SphereComp;
	// ������ �κ��丮 ������ �ݸ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* ItemInvenCol;

	// �÷��̾� �տ��ִ°�.
	UPROPERTY(EditAnywhere)
	bool bIsInHand = false;

	// �κ� ������
	UFUNCTION()
	void OnOverlapInven(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// �� ������
	UFUNCTION()
	void OnOverlapHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ������ ��������.
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// �κ��丮�� ������� ������ ����
	UFUNCTION()
	void SetPhysicsOff();

	UPROPERTY()
	class AEnemy_Skeleton* SkelEnemy;

	UPROPERTY()
	class UEnemyFSM* FSMEnemy;

	// MainPlayer
	UPROPERTY(EditAnywhere)
	class AMainPlayer* MainPlayer;
	// ItemInventory
	UPROPERTY(EditAnywhere)
	class AItemInventory* ItemInventory;
	UPROPERTY(EditAnywhere)
	bool bIsOverlapRightHand = false;
	UPROPERTY(EditAnywhere)
	bool bIsOverlapLeftHand = false;

	// �������̸�
// 	UPROPERTY(EditAnywhere)
// 	FString ItemName;
	// ������ ����
	UPROPERTY(EditAnywhere)
	FString ItemDesc;
	// ������ ����
	UPROPERTY(EditAnywhere)
	int ItemPrice;
};
