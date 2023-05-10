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

	// FSM ���� �޾ƿ��� �մ� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECustomerState OwnerState;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
// 	ECustomerSitState ownerSitState;
	
	// ��Ÿ��
	UPROPERTY(EditAnywhere)
	class UAnimMontage* CustomerMontageFactory;

	// stand ���� ��Ÿ�� �÷��� �Լ�
	UFUNCTION()
	void OnStandAnim(FName sectionName);

	// �Ҹ��� ǥ���ϴ� �ִϸ��̼� ������ �̾����� �Լ�
	UFUNCTION(BlueprintCallable)
	void EndWaitLong();

	// �ֹ��� �ϴ� �ִϸ��̼� ������ �̾����� �Լ�
	UFUNCTION(BlueprintCallable)
	void EndOrder();

	UFUNCTION(BlueprintCallable)
	void EndAngry();

	UFUNCTION(BlueprintCallable)
	void EndGood();

	UFUNCTION(BlueprintCallable)
	void EndLeave();
};
