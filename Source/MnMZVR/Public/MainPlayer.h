// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MainPlayer.generated.h"

UCLASS()
class MNMZVR_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 플레이어 이동속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	float PlayerSpeed = 600.0f;
	// 플레이어 인풋 맵핑컨텍스트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* IMC_PlayerInput;
	// 인풋 액션
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_Move;
	// 플레이어 마우스 입력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_Mouse;

	//===========플레이어 손===========//
	// 왼손
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
	class UMotionControllerComponent* LeftHand;
	// 오른손
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
	class UMotionControllerComponent* RightHand;
	// 왼손 모델
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class USkeletalMeshComponent* LeftHandMesh;
	// 오른손 모델
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class USkeletalMeshComponent* RightHandMesh;

	//=================================//


	// 이동처리함수
	void Move(const FInputActionValue& Values);
	// 회전처리 함수
	void Turn(const FInputActionValue& Values);


protected:
// 플레이어 카메라
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRCamera")
	class UCameraComponent* PlayerCamera;
};
