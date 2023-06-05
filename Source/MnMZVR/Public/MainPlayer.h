// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/ChildActorComponent.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HandComp")
	class UMotionControllerComponent* LeftAim;
	// �޼� ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class USkeletalMeshComponent* LeftHandMesh;
	// ������ ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class USkeletalMeshComponent* RightHandMesh;

	// ������ �ݸ������Ǿ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class USphereComponent* RightHandSphere;
	// �޼� �ݸ������Ǿ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class USphereComponent* LeftHandSphere;

	// �޼� �ݸ����ڽ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand")
	class UBoxComponent* LeftHandBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* BeltMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class AWeaponInventory* WeaponInven;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HandVelocity;

	FVector RightPreviousPosition;
	float RightPreviousTime;

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

	UPROPERTY()
	class AActor* GrabbedActorRight;
	// �޼� �׷� ����
	UPROPERTY()
	class AActor* GrabbedActorLeft;
	// ���� ��ü ���
	UPROPERTY()
	class UPrimitiveComponent* GrabbedObject;

	// ���� ���� ��ü ���
	UPROPERTY()
	class UPrimitiveComponent* LeftGrabbedObject;


	
	//���� bool ����
	bool IsGrabedLeft = false;
	// �޼� �׷� Ʈ���� on/off Ȯ��
	bool LeftGrabOn = false;
	//���� bool ����
	bool IsGrabedRight = false;
	// ������ �׷� Ʈ���� on/off Ȯ��
	bool RightGrabOn = false;
	
	bool bIsLeftHandinWeaponInven = false;
	bool bIsRightHandinWeaponInven = false;

	// ���� bool ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab")
	bool IsWeapon = false;

	// ������ ���ϴ� �������� ���ư����� �ϰ�ʹ�.
	// �ʿ�Ӽ� : ���� ��, ���� ����, ������ġ
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
		float MyThrowPower = 10000.0f;
	// ���� ����
	FVector ThrowDirection;
	// ���� ���� ����
	FVector LeftThrowDirection;
	// ������ġ
	FVector PrevPos;
	// �޼�������ġ
	FVector LeftPrevPos;
	// ���� ȸ����
	FQuat PrevRot;
	// ���� ���� ȸ����
	FQuat LeftPrevRot;
	// ȸ������
	FQuat DeltaRotation;
	// ���� ȸ������
	FQuat LeftDeltaRotation;
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
	FVector LastGrabbedObjectPositionLeft;

	float CurrentGrabbedObjectVelocity;

	// GrabPoint HandOffset
	UPROPERTY(EditAnywhere, Category = "Grab")
	FVector HandOffset = FVector(0, 0, 0);
	//============���==============//
	// ��� �õ� ���(����)`
	void TryGrabLeft();
	// �κ��丮 ��� �õ� ���(����)
	void TryGrabLeftInven();
	// ��� �õ� ���(������)
	void TryGrabRight();
	// ��� �õ� ���(������)
	void TryGrabRightInven();
	// �� ����
	void UnTryGrabLeft();
	// �� ����
	void UnTryGrabRight();

	//=============����==============//
	public:
	// ���� ���� �Ӽ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly ,Category = "Widget")
		class UWidgetInteractionComponent* UIInteractRight;

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
	void RemoteGrabLeft();

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

	// �÷��̾� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	int32 PlayerMoney;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* MainWidgetComp;

	//=================================//


	// ����ִ���
	void Grabbing();
	// �̵�ó���Լ�
	void Move(const FInputActionValue& Values);
	// ȸ��ó�� �Լ�
	void Turn(const FInputActionValue& Values);
	// Ŭ�� �Լ�
	void OnClick(const FInputActionValue& Values);
	
	//===============�κ��丮==================//
	// �κ��丮 �����Լ�
	UFUNCTION()
	void AttachWeaponInventory();
	// �����۰��� �����Լ�
	UFUNCTION()
	void AttachItemInventory();

	// �κ��丮 ���ڸ� ������ �Լ�
	void ItemInventoryPositionReset();
	// ItemBag Reset Position
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_ItemInvenSetPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class AItemInventory* ItemInven;

	bool bIsSwingingMeleeWeapon;


	//====================================//
public:
	// ��/�޼� ��ü �ݸ����� �κ��丮�� ������ �Ǿ��°�.
	bool bIsOverlappedRight;
	bool bIsOverlappedLeft;


	//================�÷��̾� ������ ������ ����(���Ǿ�)==================//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* ItemCheckSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_Getitem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_Menu;

	// �������Ʈ���� ������ ������ �������Լ�
	UFUNCTION(BlueprintImplementableEvent)
	void GetItem();

	// �������Ʈ���� ������ �޴�Ű�� �Լ�
	UFUNCTION(BlueprintImplementableEvent)
	void MenuOn();

protected:
// �÷��̾� ī�޶�
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VRCamera")
	class UCameraComponent* PlayerCamera;

};
