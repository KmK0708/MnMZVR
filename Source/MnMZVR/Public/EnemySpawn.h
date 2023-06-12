// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFSM.h"
#include "EnemySpawn.generated.h"

UCLASS()
class MNMZVR_API AEnemySpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// 에너미 스폰
UPROPERTY(EditAnywhere, Category = "Spawning")
TSubclassOf<class AEnemy_Skeleton> EnemyToSpawn;

// 에너미
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
class AEnemy_Skeleton* Enemy;

// 에너미 FSM
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
class UEnemyFSM* EnemyFSM;

// 스폰 딜레이
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
float SpawnDelay;

// 스폰 타이머
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
FTimerHandle SpawnTimer;

// 플레이어
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
class AMainPlayer* Player;

// 플레이어 감지 범위
UPROPERTY(EditAnywhere, Category = "Spawning")
float PlayerDetectionRange = 600.0f;

// 플레이어 스폰 범위
UPROPERTY(EditAnywhere, Category = "Spawning")
float PlayerSpawnRange = 150.0f;

// 콜리전박스
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
class UBoxComponent* BoxComp;

 UPROPERTY(EditAnywhere, Category = "Spawner")
 int32 NumberOfEnemiesToSpawn;


// 오버랩
UFUNCTION()
void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult);

UFUNCTION()
void SpawnEnemies();


};
