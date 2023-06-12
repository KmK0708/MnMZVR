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

// ���ʹ� ����
UPROPERTY(EditAnywhere, Category = "Spawning")
TSubclassOf<class AEnemy_Skeleton> EnemyToSpawn;

// ���ʹ�
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
class AEnemy_Skeleton* Enemy;

// ���ʹ� FSM
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
class UEnemyFSM* EnemyFSM;

// ���� ������
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
float SpawnDelay;

// ���� Ÿ�̸�
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
FTimerHandle SpawnTimer;

// �÷��̾�
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
class AMainPlayer* Player;

// �÷��̾� ���� ����
UPROPERTY(EditAnywhere, Category = "Spawning")
float PlayerDetectionRange = 600.0f;

// �÷��̾� ���� ����
UPROPERTY(EditAnywhere, Category = "Spawning")
float PlayerSpawnRange = 150.0f;

// �ݸ����ڽ�
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
class UBoxComponent* BoxComp;

 UPROPERTY(EditAnywhere, Category = "Spawner")
 int32 NumberOfEnemiesToSpawn;


// ������
UFUNCTION()
void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult);

UFUNCTION()
void SpawnEnemies();


};
