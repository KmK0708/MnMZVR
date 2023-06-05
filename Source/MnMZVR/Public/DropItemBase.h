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

	// 아이템 메시
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ItemMesh;
	// 손에 닿았는지 스피어콜리전
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USphereComponent* SphereComp;
	// 아이템 인벤토리 오버랩 콜리전
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* ItemInvenCol;

	// 플레이어 손에있는가.
	UPROPERTY(EditAnywhere)
	bool bIsInHand = false;

	// 인벤 오버랩
	UFUNCTION()
	void OnOverlapInven(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 손 오버랩
	UFUNCTION()
	void OnOverlapHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 오버랩 끝났을때.
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 인벤토리에 담겼을시 피직스 끄기
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

	// 아이템이름
// 	UPROPERTY(EditAnywhere)
// 	FString ItemName;
	// 아이템 설명
	UPROPERTY(EditAnywhere)
	FString ItemDesc;
	// 아이템 가격
	UPROPERTY(EditAnywhere)
	int ItemPrice;
};
