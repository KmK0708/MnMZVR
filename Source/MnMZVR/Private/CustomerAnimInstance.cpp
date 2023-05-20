// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerAnimInstance.h"

#include "CustomerCharacter.h"
#include "CustomerFSM.h"

void UCustomerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCustomerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Owner == nullptr)
	{
		return;
	}

	// 속도를 받아온다
	FVector Velocity = Owner->GetVelocity();
	FVector forVelocity = Owner->GetActorForwardVector();
	forwardVelocity = FVector::DotProduct(Velocity, forVelocity);
}

void UCustomerAnimInstance::OnStandAnim(FName sectionName)
{
	// Owner->PlayAnimMontage(CustomerMontageFactory, 1, sectionName);
}

void UCustomerAnimInstance::EndWaitLong()
{
// 	if (Owner->CustomerFSM->State == ECustomerState::STANDBYWAITLONG)
// 	{
// 		Owner->CustomerFSM->SetState(ECustomerState::STANDBYWAITLONG);
// 	}
// 	else
// 	{
// 		Owner->CustomerFSM->SetState(ECustomerState::WAITLONG);
// 	}
}

void UCustomerAnimInstance::EndOrder()
{
	// Owner->CustomerFSM->SetState(ECustomerState::WAIT);
}

void UCustomerAnimInstance::EndAngry()
{
// 	if (Owner->CustomerFSM->spawnManager->aWaitingSpot[Owner->CustomerFSM->idx]->bSameOrder == true)
// 	{
// 		Owner->CustomerFSM->SetState(ECustomerState::LEAVE);
// 	}
// 	else if (Owner->CustomerFSM->spawnManager->aChairs[Owner->CustomerFSM->idx]->bUnSameOrder == true)
// 	{
// 		if (Owner->CustomerFSM->bCheckOrder != true)
// 		{
// 			Owner->CustomerFSM->SetState(ECustomerState::ORDERJUDGE);
// 		}
// 		else
// 		{
// 			Owner->CustomerFSM->SetState(ECustomerState::LEAVE);
// 		}
// 	}
}

void UCustomerAnimInstance::EndGood()
{
	// Owner->CustomerFSM->SetState(ECustomerState::LEAVE);
}

void UCustomerAnimInstance::EndLeave()
{
	// Owner->CustomerFSM->SetState(ECustomerState::LEAVE);
}
