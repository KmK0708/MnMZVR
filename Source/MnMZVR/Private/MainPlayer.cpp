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
	// 카메라 설정
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetRelativeLocation(FVector(0, 0, 64));
	PlayerCamera->bUsePawnControlRotation = true;

	// 왼손 추가
	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(RootComponent);
	LeftHand->SetTrackingMotionSource(FName("Left"));
	// 오른손
	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RootComponent);
	RightHand->SetTrackingMotionSource(FName("Right"));

	// 왼손 스켈레탈 메시 설정
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftHand);
	// 스켈레탈 메시 로드 후 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh> LHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (LHandMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(LHandMesh.Object);
		LeftHandMesh->SetRelativeLocation(FVector(-2.9f, -3.5f, 4.5f));
		LeftHandMesh->SetRelativeRotation(FRotator(-25, -180, 90));
		// (X=-2.981260,Y=-3.500000,Z=4.561753)
		// (Pitch=-25.000000,Yaw=-179.999999,Roll=89.999998)
	}

	// 오른손 스켈레탈 메시 설정
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightHand);
	// 스켈레탈 메시 로드 후 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh> RHandMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (RHandMesh.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh(RHandMesh.Object);
		RightHandMesh->SetRelativeLocation(FVector(-2.9f, 3.5f, 4.5f));
		RightHandMesh->SetRelativeRotation(FRotator(25, 0, 90));
		// (X=-2.981260,Y=3.500000,Z=4.561753)
		// (Pitch=25.000000,Yaw=0.000000,Roll=89.999999)
	}

	// 집게손가락
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
	
	// Enhanced input 사용
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

	// HMD 가 연결되어 있지 않으면
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		// Hand 를 테스트 할 수 있는 위치로 이동시키자.
		RightHand->SetRelativeLocation(FVector(20, 20, 0));
		// 에임도 일치하게 하자
		RightAim->SetRelativeLocation(FVector(20, 20, 0));
		// 카메라의 Use Pawn Control Rotation 을 활성화 시키자.
		PlayerCamera->bUsePawnControlRotation = true;
	}
	// 만약 HMD가 연결되어 있다면.
	else
	{
		// -> 기본 트랙킹 offset 설정
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
		PlayerCamera->bUsePawnControlRotation = false;
	}
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// HMD 가 연결돼 있지 않으면
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		//  -> 손이 카메라 방향과 일치하도록 하자
		RightHand->SetRelativeRotation(PlayerCamera->GetRelativeRotation());
		// 에임도 일치하게 하자
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
	// 사용자의 입력에따라 앞뒤좌우로 이동하고 싶다.
	// 1. 사용자의 입력에 따라
	FVector2D Axis = Values.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("move"));
	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);
	// 2. 앞뒤좌우라는 방향이 필요.
// 	FVector Dir(Axis.X, Axis.Y, 0);
// 	// 3. 이동하고싶다.
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
// 	// UI 에 이벤트를 전달하고 싶다.
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
{	// 잡은 물체가 있다면
	if (IsGrabedLeft == false)
	{
		return;
	}


	// 1. 잡지않은 상태로 전환
	IsGrabedLeft = false;
	// 2. 손에서 떼어주기
	GrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 3. 물리기능 활성화
	GrabbedObject->SetSimulatePhysics(true);
	// 4. 충돌기능 활성화
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// 던지기
	GrabbedObject->AddForce(ThrowDirection * MyThrowPower * GrabbedObject->GetMass());

	// 회전시키기
	GrabbedObject->AddTorqueInRadians(DeltaRotation.Euler() * MyThrowPower * GrabbedObject->GetMass());
	// 각속도 = (1 / dt) * dTheta(특정 축 기준 변위 각도 Axis, angle)
	float Angle;
	FVector Axis;
	DeltaRotation.ToAxisAndAngle(Axis, Angle);
	float dt = GetWorld()->DeltaTimeSeconds;
	FVector AngularVelocity = (1 / dt) * Angle * Axis;
	GrabbedObject->SetPhysicsAngularVelocityInRadians(AngularVelocity * TorquePower, true);
	// 잡은 물체 참조 초기화
	GrabbedObject = nullptr;
}

void AMainPlayer::UnTryGrabRight()
{
}

void AMainPlayer::Grabbing()
{
	// 잡은 물체가 있다면
	if (IsGrabedLeft == false)
	{
		return;
	}
	// ↓ 놓았을때 기능
	// 던질 방향 업데이트
	ThrowDirection = RightHand->GetComponentLocation() - PrevPos;
	// 회전 방향 업데이트
	// 쿼터니언 공식
	// Angle1 = Q1 , Angle2 = Q2
	// Angle1 + Angle2 = Q1 * Q2
	// -Angle1 = Q2.Inverse()
	// Angle2 - Angle1 = Q2 * Q1.Inverse()
	DeltaRotation = RightHand->GetComponentQuat() * PrevRot.Inverse();


	// 이전 위치 업데이트
	PrevPos = RightHand->GetComponentLocation();
	// 이전회전값 업데이트
	PrevRot = RightHand->GetComponentQuat();
}

void AMainPlayer::ReleaseUIInput()
{
// 	// UI 에 이벤트를 전달하고 싶다.
// 	if (WidgetInteractionComp)
// 	{
// 		//WidgetInteractionComp->PressPointerKey(FKey(FName("LeftMouseButton")));
// 		WidgetInteractionComp->ReleasePointerKey(EKeys::LeftMouseButton);
// 	}
}

