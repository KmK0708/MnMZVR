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
	// 손 맵핑컨텍스트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* IMC_Hand;
	// 인풋 액션
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_Move;
	// 플레이어 마우스 입력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_Mouse;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_FireNClick;


	//===========플레이어 손===========//
	// 왼손
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
	class UMotionControllerComponent* LeftHand;
	// 오른손
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
	class UMotionControllerComponent* RightHand;
	// 집게손가락 표시할 모션컨트롤러
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HandComp")
	class UMotionControllerComponent* RightAim;
	// 왼손 모델
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class USkeletalMeshComponent* LeftHandMesh;
	// 오른손 모델
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class USkeletalMeshComponent* RightHandMesh;

	// 오른손 콜리전박스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class UBoxComponent* RightHandBox;
	// 왼손 콜리전박스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class UBoxComponent* LeftHandBox;

	//=============물체잡기=============//
	// 필요속성 : 입력액션, 잡을 범위
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Grab_L;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Grab_R;
	// 잡을 범위
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
		float GrabRange = 50.0f;
	// 잡을 최대 범위
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
		float GrabMaxRange = 50.0f;
	// 잡은 물체 기억
	UPROPERTY()
	class UPrimitiveComponent* GrabbedObject;
	
	//왼잡 bool 변수
	bool IsGrabedLeft = false;
	//오잡 bool 변수
	bool IsGrabedRight = false;
	
	// 무기 bool 변수
	bool IsWeapon = false;

	// 던지면 원하는 방향으로 날아가도록 하고싶다.
	// 필요속성 : 던질 힘, 던질 방향, 직전위치
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
		float MyThrowPower = 10000.0f;
	// 던질 방향
	FVector ThrowDirection;
	// 직전위치
	FVector PrevPos;
	// 이전 회전값
	FQuat PrevRot;
	// 회전방향
	FQuat DeltaRotation;
	// 회전빠르기
	UPROPERTY(EditAnywhere, Category = "Grab")
	float TorquePower = 1;
	// 공격 휘두르기 한계점
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeWeapon")
	float MinSwingSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    class AMeleeWeaponBase* Weapon;

	UPROPERTY()
	FVector LastGrabbedObjectPosition;

	UPROPERTY()
    class UWeaponInvenComp* WeaponInvenComp;

	float CurrentGrabbedObjectVelocity;
	//=============위젯==============//
	protected:
	// 위젯 관련 속성
	UPROPERTY(VisibleAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		class UWidgetInteractionComponent* WidgetInteractionComp;

	// 릴리즈 상태로 되돌려놓기
	void ReleaseUIInput();

		// 원격잡기 모드가 활성화 되면 원격잡기 모드 사용하도록 하고싶다.
	//필요속성 : 원격잡기 모드 여부, 거리, 이동속도
	UPROPERTY(EditDefaultsOnly, Category="Grab")
	bool IsRemoteGrab = true;
	UPROPERTY(EditDefaultsOnly, Category="Grab")
	float RemoteDistance = 2000;
	UPROPERTY(EditDefaultsOnly, Category="Grab")
	float RemoteMoveSpeed = 10;
	// 검출범위
	UPROPERTY(EditDefaultsOnly, Category="Grab")
	float RemoteRadius = 20;
	// 원거리 물체 이동을 위한 타이머
	FTimerHandle GrabTimer;

	void RemoteGrab();

	// Remote Grab 시각화 처리할지 여부
	UPROPERTY(EditDefaultsOnly, Category="Grab")
	bool bDrawDebugRemoteGrab = true;
	// 시각화 처리할 함수
	void DrawDebugRemoteGrab();

	//===========플레이어 정보===========//
	// 플레이어 HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	float PlayerHP;
	// 플레이어 최대 HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	float PlayerMaxHP = 100.0f;


	//=================================//

	// 잡기 시도 기능(왼쪽)
	void TryGrabLeft();
	// 잡기 시도 기능(오른쪽)
	void TryGrabRight();
	// 왼 놓기
	void UnTryGrabLeft();
	// 오 놓기
	void UnTryGrabRight();
	// 잡고있는중
	void Grabbing();
	// 이동처리함수
	void Move(const FInputActionValue& Values);
	// 회전처리 함수
	void Turn(const FInputActionValue& Values);
	// 클릭 함수
	void OnClick(const FInputActionValue& Values);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartSwingingMeleeWeapon();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopSwingingMeleeWeapon();

	bool bIsSwingingMeleeWeapon;

protected:
// 플레이어 카메라
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRCamera")
	class UCameraComponent* PlayerCamera;

};
