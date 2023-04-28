// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestEnemy.generated.h"

UCLASS()
class MNMZVR_API ATestEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestEnemy();

		// 적 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySettings")
		class USkeletalMeshComponent* EnemyMesh;

		// 적 래그돌
	UFUNCTION(BlueprintCallable, Category = "EnemySettings")
		void OnDeath();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
		float EnemyHealth = 1000.0f;
	// 최대체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
	float EnemyMaxHealth = 1000.0f;

	// 죽었을떄.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
		bool bIsDead = false;

	// 체력설정함수
	UFUNCTION(BlueprintCallable, Category = "EnemySettings")
		void AddHealth(float Health);
};
