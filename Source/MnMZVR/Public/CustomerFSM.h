// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomerFSM.generated.h"

UENUM(BlueprintType)
enum class ECustomerState : uint8
{
	IDLE,
	MOVE,
	WAIT,
	WAITLONG,
	ANGRY,
	GOOD,
	LEAVE,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MNMZVR_API UCustomerFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomerFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// �մ� �⺻ ����
	ECustomerState State;

	UPROPERTY()
	class ACustomerCharacter* Owner;

// 	UPROPERTY()
// 	class ASpawnManager* SpawnManager;

	// ��� ���� ���� ����
	UPROPERTY()
	int32 idx;

	// �ֹ� ���� ����
	UPROPERTY()
	int32 orderIdx;

	// �ð�
	float curTime;

	// �ִϸ��̼� �÷��� ����
	bool bCheckPlayAnim;

	// �ֹ� ���н� ��ȸ �Ҹ� ����
	bool bCheckOrder;
	
	class AAIController* AI;

	// �ֹ� Ĭ���� ���ϱ�
	void SetOrderitem();

	// ���� ��ȯ �Լ�
	UFUNCTION()
	void SetState(ECustomerState next);

	// ���� �Լ�
	UFUNCTION()
	int32 SetRandRange(int32 idxStart, int32 idxEnd);

private:
	//�մ� ���� �Լ�
	void Idle();

	void Move();

	void Wait();

	void WaitLong();

	void Angry();

	void Good();

	void Leave();
};
