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


	// �÷��̾� �̵��ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	float PlayerSpeed = 600.0f;
	// �÷��̾� ��ǲ �������ؽ�Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* IMC_PlayerInput;
	// �� �������ؽ�Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* IMC_Hand;
	// ��ǲ �׼�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_Move;
	// �÷��̾� ���콺 �Է�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_Mouse;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_FireNClick;


	//===========�÷��̾� ��===========//
	// �޼�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
	class UMotionControllerComponent* LeftHand;
	// ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
	class UMotionControllerComponent* RightHand;
	// ���Լհ��� ǥ���� �����Ʈ�ѷ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HandComp")
	class UMotionControllerComponent* RightAim;
	// �޼� ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class USkeletalMeshComponent* LeftHandMesh;
	// ������ ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class USkeletalMeshComponent* RightHandMesh;

	// ������ �ݸ����ڽ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class UBoxComponent* RightHandBox;
	// �޼� �ݸ����ڽ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class UBoxComponent* LeftHandBox;

	//=============��ü���=============//
	// �ʿ�Ӽ� : �Է¾׼�, ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Grab_L;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		class UInputAction* IA_Grab_R;
	// ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
		float GrabRange = 50.0f;
	// ���� �ִ� ����
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
		float GrabMaxRange = 50.0f;
	// ���� ��ü ���
	UPROPERTY()
	class UPrimitiveComponent* GrabbedObject;
	
	//���� bool ����
	bool IsGrabedLeft = false;
	//���� bool ����
	bool IsGrabedRight = false;
	
	// ���� bool ����
	bool IsWeapon = false;

	// ������ ���ϴ� �������� ���ư����� �ϰ�ʹ�.
	// �ʿ�Ӽ� : ���� ��, ���� ����, ������ġ
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
		float MyThrowPower = 10000.0f;
	// ���� ����
	FVector ThrowDirection;
	// ������ġ
	FVector PrevPos;
	// ���� ȸ����
	FQuat PrevRot;
	// ȸ������
	FQuat DeltaRotation;
	// ȸ��������
	UPROPERTY(EditAnywhere, Category = "Grab")
	float TorquePower = 1;
	// ���� �ֵθ��� �Ѱ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeWeapon")
	float MinSwingSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    class AMeleeWeaponBase* Weapon;

	UPROPERTY()
	FVector LastGrabbedObjectPosition;

	UPROPERTY()
    class UWeaponInvenComp* WeaponInvenComp;

	float CurrentGrabbedObjectVelocity;
	//=============����==============//
	protected:
	// ���� ���� �Ӽ�
	UPROPERTY(VisibleAnywhere, Category = "Widget", meta = (AllowPrivateAccess = true))
		class UWidgetInteractionComponent* WidgetInteractionComp;

	// ������ ���·� �ǵ�������
	void ReleaseUIInput();

		// ������� ��尡 Ȱ��ȭ �Ǹ� ������� ��� ����ϵ��� �ϰ�ʹ�.
	//�ʿ�Ӽ� : ������� ��� ����, �Ÿ�, �̵��ӵ�
	UPROPERTY(EditDefaultsOnly, Category="Grab")
	bool IsRemoteGrab = true;
	UPROPERTY(EditDefaultsOnly, Category="Grab")
	float RemoteDistance = 2000;
	UPROPERTY(EditDefaultsOnly, Category="Grab")
	float RemoteMoveSpeed = 10;
	// �������
	UPROPERTY(EditDefaultsOnly, Category="Grab")
	float RemoteRadius = 20;
	// ���Ÿ� ��ü �̵��� ���� Ÿ�̸�
	FTimerHandle GrabTimer;

	void RemoteGrab();

	// Remote Grab �ð�ȭ ó������ ����
	UPROPERTY(EditDefaultsOnly, Category="Grab")
	bool bDrawDebugRemoteGrab = true;
	// �ð�ȭ ó���� �Լ�
	void DrawDebugRemoteGrab();

	//===========�÷��̾� ����===========//
	// �÷��̾� HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	float PlayerHP;
	// �÷��̾� �ִ� HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	float PlayerMaxHP = 100.0f;


	//=================================//

	// ��� �õ� ���(����)
	void TryGrabLeft();
	// ��� �õ� ���(������)
	void TryGrabRight();
	// �� ����
	void UnTryGrabLeft();
	// �� ����
	void UnTryGrabRight();
	// ����ִ���
	void Grabbing();
	// �̵�ó���Լ�
	void Move(const FInputActionValue& Values);
	// ȸ��ó�� �Լ�
	void Turn(const FInputActionValue& Values);
	// Ŭ�� �Լ�
	void OnClick(const FInputActionValue& Values);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartSwingingMeleeWeapon();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopSwingingMeleeWeapon();

	bool bIsSwingingMeleeWeapon;

protected:
// �÷��̾� ī�޶�
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRCamera")
	class UCameraComponent* PlayerCamera;

};
