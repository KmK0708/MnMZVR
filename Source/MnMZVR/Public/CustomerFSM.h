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

	// 손님 기본 상태
	ECustomerState State;

	UPROPERTY()
	class ACustomerCharacter* Owner;

// 	UPROPERTY()
// 	class ASpawnManager* SpawnManager;

	// 대기 순서 저장 인자
	UPROPERTY()
	int32 idx;

	// 주문 저장 인자
	UPROPERTY()
	int32 orderIdx;

	// 시간
	float curTime;

	// 애니메이션 플레이 여부
	bool bCheckPlayAnim;

	// 주문 실패시 기회 소모 여부
	bool bCheckOrder;
	
	class AAIController* AI;

	// 주문 칵테일 정하기
	void SetOrderitem();

	// 상태 전환 함수
	UFUNCTION()
	void SetState(ECustomerState next);

	// 랜덤 함수
	UFUNCTION()
	int32 SetRandRange(int32 idxStart, int32 idxEnd);

private:
	//손님 상태 함수
	void Idle();

	void Move();

	void Wait();

	void WaitLong();

	void Angry();

	void Good();

	void Leave();
};
