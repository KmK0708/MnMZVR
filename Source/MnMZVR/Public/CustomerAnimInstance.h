// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CustomerFSM.h"
#include "CustomerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MNMZVR_API UCustomerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class ACustomerCharacter* Owner;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float forwardVelocity;

	// FSM 에서 받아오는 손님 상태
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECustomerState OwnerState;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
// 	ECustomerSitState ownerSitState;
	
	// 몽타주
	UPROPERTY(EditAnywhere)
	class UAnimMontage* CustomerMontageFactory;

	// stand 상태 몽타주 플레이 함수
	UFUNCTION()
	void OnStandAnim(FName sectionName);

	// 불만을 표출하는 애니메이션 끝나고 이어지는 함수
	UFUNCTION(BlueprintCallable)
	void EndWaitLong();

	// 주문을 하는 애니메이션 끝나고 이어지는 함수
	UFUNCTION(BlueprintCallable)
	void EndOrder();

	UFUNCTION(BlueprintCallable)
	void EndAngry();

	UFUNCTION(BlueprintCallable)
	void EndGood();

	UFUNCTION(BlueprintCallable)
	void EndLeave();
};
