// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerFSM.h"

#include "AIController.h"
#include "MainPlayer.h"
#include "CustomerAnimInstance.h"
#include "CustomerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCustomerFSM::UCustomerFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCustomerFSM::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACustomerCharacter>(GetOwner());

	AI = Cast<AAIController>(Owner->GetController());

	//SpawnManager = Cast<ASpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnManager::StaticClass()));
	
	// 기본 상태 설정
	State = ECustomerState::IDLE;

	// 걷기 속도 조절
	Owner->GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	// 주문 칵테일 정하기
	SetOrderitem();
}


// Called every frame
void UCustomerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{
	case ECustomerState::IDLE:
		Idle();
		break;
	case ECustomerState::MOVE:
		Move();
		break;
	case ECustomerState::WAIT:
		Wait();
		break;
	case ECustomerState::WAITLONG:
		WaitLong();
		break;
	case ECustomerState::ANGRY:
		Angry();
		break;
	case ECustomerState::GOOD:
		Good();
		break;
	case ECustomerState::LEAVE:
		Leave();
		break;
	}
}

void UCustomerFSM::SetOrderitem()
{
	if (Owner != nullptr)
	{
		// 랜덤으로 아이템 선정
		int32 result = FMath::RandRange(1, 8);

		if (result > 6)
		{
			// 마석
			orderIdx = 1;
		}
		else if (result <= 6 && result > 4)
		{
			// 철광석
			//orderIdx = 2;
			orderIdx = 1;
		}
		else if (result <= 4 && result > 2)
		{
			// 의문의 물약
			//orderIdx = 3;
			orderIdx = 1;
		}
		else if (result <= 2)
		{
			// (나머지 정해지면)
			//orderIdx = 4;
			orderIdx = 1;
		}
	}
}

void UCustomerFSM::SetState(ECustomerState next)
{
	State = next;

	// 플레이 되는 애니메이션 플레이 체크 초기화
	bCheckPlayAnim = false;

	// 상태를 anim 클래스에 동기화
	//Owner->CustomerAnim->OwnerState = next;
}

int32 UCustomerFSM::SetRandRange(int32 idxStart, int32 idxEnd)
{
	return int32();
}

void UCustomerFSM::Idle()
{
}

void UCustomerFSM::Move()
{
}

void UCustomerFSM::Wait()
{
}

void UCustomerFSM::WaitLong()
{
}

void UCustomerFSM::Angry()
{
}

void UCustomerFSM::Good()
{
}

void UCustomerFSM::Leave()
{
}

