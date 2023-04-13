// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include <Camera/CameraComponent.h>
#include <MotionControllerComponent.h>
#include <HeadMountedDisplayFunctionLibrary.h>
#include <Components/CapsuleComponent.h>
#include <UMG/Public/Components/WidgetInteractionComponent.h>

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// ī�޶� ����
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetRelativeLocation(FVector(0, 0, 64));
	PlayerCamera->bUsePawnControlRotation = true;

	// �޼� �߰�
	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(RootComponent);
	LeftHand->SetTrackingMotionSource(FName("Left"));
	// ������
	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RootComponent);
	RightHand->SetTrackingMotionSource(FName("Right"));

	// �޼� ���̷�Ż �޽� ����
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftHand);
	// ���̷�Ż �޽� �ε� �� �Ҵ�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> LHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (LHandMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(LHandMesh.Object);
		LeftHandMesh->SetRelativeLocation(FVector(-2.9f, -3.5f, 4.5f));
		LeftHandMesh->SetRelativeRotation(FRotator(-25, -180, 90));
		// (X=-2.981260,Y=-3.500000,Z=4.561753)
		// (Pitch=-25.000000,Yaw=-179.999999,Roll=89.999998)
	}

	// ������ ���̷�Ż �޽� ����
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightHand);
	// ���̷�Ż �޽� �ε� �� �Ҵ�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> RHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (RHandMesh.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh(RHandMesh.Object);
		RightHandMesh->SetRelativeLocation(FVector(-2.9f, 3.5f, 4.5f));
		RightHandMesh->SetRelativeRotation(FRotator(25, 0, 90));
		// (X=-2.981260,Y=3.500000,Z=4.561753)
		// (Pitch=25.000000,Yaw=0.000000,Roll=89.999999)
	}

	// ���Լհ���
	RightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightAim"));
	RightAim->SetupAttachment(RootComponent);
	RightAim->SetTrackingMotionSource(FName("RightAim"));

//  	WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComp"));
//  	WidgetInteractionComp->SetupAttachment(RightAim);
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Enhanced input ���
	auto PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC)
	{
		// LocalPlayer
		auto localPlayer = PC->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
		if (subSystem)
		{
			subSystem->AddMappingContext(IMC_PlayerInput, 0);
			subSystem->AddMappingContext(IMC_Hand, 0);
		}
	}

	// HMD �� ����Ǿ� ���� ������
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		// Hand �� �׽�Ʈ �� �� �ִ� ��ġ�� �̵���Ű��.
		RightHand->SetRelativeLocation(FVector(20, 20, 0));
		// ���ӵ� ��ġ�ϰ� ����
		RightAim->SetRelativeLocation(FVector(20, 20, 0));
		// ī�޶��� Use Pawn Control Rotation �� Ȱ��ȭ ��Ű��.
		PlayerCamera->bUsePawnControlRotation = true;
	}
	// ���� HMD�� ����Ǿ� �ִٸ�.
	else
	{
		// -> �⺻ Ʈ��ŷ offset ����
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
		PlayerCamera->bUsePawnControlRotation = false;
	}
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// HMD �� ����� ���� ������
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		//  -> ���� ī�޶� ����� ��ġ�ϵ��� ����
		RightHand->SetRelativeRotation(PlayerCamera->GetRelativeRotation());
		// ���ӵ� ��ġ�ϰ� ����
		RightAim->SetRelativeRotation(PlayerCamera->GetRelativeRotation());

	}

	// Grabbing
	Grabbing();
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent)
	{
		enhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMainPlayer::Move);
		enhancedInputComponent->BindAction(IA_Mouse, ETriggerEvent::Triggered, this, &AMainPlayer::Turn);
		enhancedInputComponent->BindAction(IA_FireNClick, ETriggerEvent::Started, this, &AMainPlayer::OnClick);
		enhancedInputComponent->BindAction(IA_FireNClick, ETriggerEvent::Completed, this, &AMainPlayer::ReleaseUIInput);
		enhancedInputComponent->BindAction(IA_Grab_L, ETriggerEvent::Started, this, &AMainPlayer::TryGrabLeft);
		enhancedInputComponent->BindAction(IA_Grab_L, ETriggerEvent::Completed, this, &AMainPlayer::UnTryGrabLeft);
		enhancedInputComponent->BindAction(IA_Grab_R, ETriggerEvent::Started, this, &AMainPlayer::TryGrabRight);
		enhancedInputComponent->BindAction(IA_Grab_R, ETriggerEvent::Completed, this, &AMainPlayer::UnTryGrabRight);
	}
}

void AMainPlayer::Move(const FInputActionValue& Values)
{
	// ������� �Է¿����� �յ��¿�� �̵��ϰ� �ʹ�.
	// 1. ������� �Է¿� ����
	FVector2D Axis = Values.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("move"));
	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);
	// 2. �յ��¿��� ������ �ʿ�.
// 	FVector Dir(Axis.X, Axis.Y, 0);
// 	// 3. �̵��ϰ�ʹ�.
// 	// P = P0 + vt
// 	FVector P0 = GetActorLocation();
// 	FVector vt = Dir * PlayerSpeed * GetWorld()->DeltaTimeSeconds;
// 	FVector P = P0 + vt;
// 	SetActorLocation(P);
}

void AMainPlayer::Turn(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void AMainPlayer::OnClick(const FInputActionValue& Values)
{
// 	// UI �� �̺�Ʈ�� �����ϰ� �ʹ�.
// 	if (WidgetInteractionComp)
// 	{
// 		//WidgetInteractionComp->PressPointerKey(FKey(FName("LeftMouseButton")));
// 		WidgetInteractionComp->PressPointerKey(EKeys::LeftMouseButton);
// 	}
}

void AMainPlayer::TryGrabLeft()
{

}

void AMainPlayer::TryGrabRight()
{

}

void AMainPlayer::UnTryGrabLeft()
{	// ���� ��ü�� �ִٸ�
	if (IsGrabedLeft == false)
	{
		return;
	}


	// 1. �������� ���·� ��ȯ
	IsGrabedLeft = false;
	// 2. �տ��� �����ֱ�
	GrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 3. ������� Ȱ��ȭ
	GrabbedObject->SetSimulatePhysics(true);
	// 4. �浹��� Ȱ��ȭ
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// ������
	GrabbedObject->AddForce(ThrowDirection * MyThrowPower * GrabbedObject->GetMass());

	// ȸ����Ű��
	GrabbedObject->AddTorqueInRadians(DeltaRotation.Euler() * MyThrowPower * GrabbedObject->GetMass());
	// ���ӵ� = (1 / dt) * dTheta(Ư�� �� ���� ���� ���� Axis, angle)
	float Angle;
	FVector Axis;
	DeltaRotation.ToAxisAndAngle(Axis, Angle);
	float dt = GetWorld()->DeltaTimeSeconds;
	FVector AngularVelocity = (1 / dt) * Angle * Axis;
	GrabbedObject->SetPhysicsAngularVelocityInRadians(AngularVelocity * TorquePower, true);
	// ���� ��ü ���� �ʱ�ȭ
	GrabbedObject = nullptr;
}

void AMainPlayer::UnTryGrabRight()
{
}

void AMainPlayer::Grabbing()
{
	// ���� ��ü�� �ִٸ�
	if (IsGrabedLeft == false)
	{
		return;
	}
	// �� �������� ���
	// ���� ���� ������Ʈ
	ThrowDirection = RightHand->GetComponentLocation() - PrevPos;
	// ȸ�� ���� ������Ʈ
	// ���ʹϾ� ����
	// Angle1 = Q1 , Angle2 = Q2
	// Angle1 + Angle2 = Q1 * Q2
	// -Angle1 = Q2.Inverse()
	// Angle2 - Angle1 = Q2 * Q1.Inverse()
	DeltaRotation = RightHand->GetComponentQuat() * PrevRot.Inverse();


	// ���� ��ġ ������Ʈ
	PrevPos = RightHand->GetComponentLocation();
	// ����ȸ���� ������Ʈ
	PrevRot = RightHand->GetComponentQuat();
}

void AMainPlayer::ReleaseUIInput()
{
// 	// UI �� �̺�Ʈ�� �����ϰ� �ʹ�.
// 	if (WidgetInteractionComp)
// 	{
// 		//WidgetInteractionComp->PressPointerKey(FKey(FName("LeftMouseButton")));
// 		WidgetInteractionComp->ReleasePointerKey(EKeys::LeftMouseButton);
// 	}
}

