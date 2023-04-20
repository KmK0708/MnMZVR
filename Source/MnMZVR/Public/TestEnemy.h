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

		// �� �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySettings")
		class USkeletalMeshComponent* EnemyMesh;

		// �� ���׵�
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

	// ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
		float EnemyHealth = 1000.0f;
	// �ִ�ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
	float EnemyMaxHealth = 1000.0f;

	// �׾�����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
		bool bIsDead = false;

	// ü�¼����Լ�
	UFUNCTION(BlueprintCallable, Category = "EnemySettings")
		void AddHealth(float Health);
};
