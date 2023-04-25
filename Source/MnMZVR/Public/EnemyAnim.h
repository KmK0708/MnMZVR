// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class MNMZVR_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemystate animState;

	//���ݻ��� ������� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	bool bAttackPlay = false;
	                                             
	// ���� �ִϸ��̼� ������ �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable, Category=FSMEvent)
	void OnEndAttackAnimation();

	//// �ǰ� �ִϸ��̼� ����Լ�
	//UFUNCTION(BlueprintImplementableEvent, Category=FSMEvent)
	//void PlayDamageAnim(FName sectionName);

	//// �������� �ִϸ��̼� ���Ῡ��
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	//bool bDieDone = false;
};
