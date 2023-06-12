// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MNMZVR_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
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

	// ������� �ִϸ��̼� �������Ʈ
	UPROPERTY()
	class UEnemyAnim* anim;

	//Enemy�� �����ϰ� �ִ� AIController
	UPROPERTY()
	class AAIController* ai;

	bool bFoundAIController = false;
};
