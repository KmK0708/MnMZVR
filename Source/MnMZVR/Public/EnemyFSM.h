// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFSM.generated.h"

// ����� ���� ����
UENUM(BlueprintType)
enum class EEnemystate : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS()
class MNMZVR_API AEnemyFSM : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyFSM();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	public:
	// ���º���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemystate mState = EEnemystate::Idle;

	//������
	void IdleState();
	//�̵�����
	void MoveState();
	//���ݻ���
	void AttackState();
	//�ǰݻ���
	void DamageState();
	//��������
	void DieState();

	// ���ð�
	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float idleDelayTime = 1;
	// ����ð�
	float currentTime = 0;

	// Ÿ�������� ���Ѵ�.
	UPROPERTY(VisibleAnywhere, Category=FSM)
	class AMainPlayer* target;

	// ���� ����
	UPROPERTY()
	class AEnemy_Skeleton* me;

	// ���� ����
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackRange = 150.0f;

	// ���� ��� �ð�
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 3.0f;

	// �´� �� �˸��� �̺�Ʈ �Լ�
	void OnDamageProcess(int32 damageValue);

	//ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3;

	//�ǰ� ��� �ð�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	float damageDelayTime = 1.0f;

	//�������� �ӵ�
	UPROPERTY(EditAnywhere, Category=FSM)
	float dieSpeed = 50.0f;
	// ������� �ִϸ��̼� �������Ʈ
	UPROPERTY()
	class UEnemyAnim* anim;

	bool isdying = false;

	UPROPERTY()
	class AMnMZGameModeBase* GameMode;

	// Ai ��Ʈ��
	UPROPERTY()
	class AAIController* ai;

private:
	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();
};
