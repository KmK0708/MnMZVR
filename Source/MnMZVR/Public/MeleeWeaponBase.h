// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeaponBase.generated.h"

UCLASS()
class MNMZVR_API AMeleeWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 무기메시
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* WeaponMesh;
	// 공격이 됐는지 판정박스
	UPROPERTY(EditAnywhere)
	class UBoxComponent* AttackBox;
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereCol;

	// Damage of the weapon
	UPROPERTY(EditAnywhere)
	float MeleeDamage = 1.0f;

	// 휘두를시 사운드큐
	UPROPERTY(EditAnywhere)
	class USoundBase* SwingSound;

	// 플레이어 손에있는가.
	UPROPERTY(EditAnywhere)
	bool bIsInHand = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float WeaponVelocity;

	// 공격함수
	UFUNCTION()
	void Attack();
	// 공격끝
	UFUNCTION()
	void EndAttack();

	// 오버랩
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 오버랩 끝났을때.
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY()
	class ATestEnemy* Enemy;

	UPROPERTY()
	class AEnemy_Skeleton* SkelEnemy;

	UPROPERTY()
	class UEnemyFSM* FSMEnemy;

	// MainPlayer
	UPROPERTY()
	class AMainPlayer* MainPlayer;

	// 인벤토리
	UPROPERTY()
	class AWeaponInventory* WeaponInven;

	bool bIsOverlapRight = false;
	bool bIsOverlapLeft = false;
protected:

	UPROPERTY()
	FVector prevPos;


	UFUNCTION()
	float GetWeaponSpeed();
};
