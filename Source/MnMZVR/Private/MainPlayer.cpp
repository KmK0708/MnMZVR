// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "MeleeWeaponBase.h"
#include <Components/BoxComponent.h>
#include <DrawDebugHelpers.h>
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
	// 왼손 콜리전박스
	LeftHandBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollision"));
	LeftHandBox->SetupAttachment(LeftHandMesh);
	// 콜리전박스 크기 설정
	LeftHandBox->SetBoxExtent(FVector(4, 4, 1));
	// 콜리전박스 위치 설정
	LeftHandBox->SetRelativeLocation(FVector(-0.4f, 4.5f, 0.22f));
	LeftHandBox->SetCollisionProfileName(TEXT("BlockAll"));
	LeftHandBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftHandBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftHandBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
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

	AttackSpeedThreshold = 1000.0f;
	Weapon = CreateDefaultSubobject<AMeleeWeaponBase>(TEXT("Weapon"));
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

	// 플레이어 체력은 플레이어 맥스 체력과 동일
	PlayerHP = PlayerMaxHP;

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
	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);
}

void AMainPlayer::Turn(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void AMainPlayer::OnClick(const FInputActionValue& Values)
{
	// UI 에 이벤트를 전달하고 싶다.
	if (WidgetInteractionComp)
	{
		//WidgetInteractionComp->PressPointerKey(FKey(FName("LeftMouseButton")));
		WidgetInteractionComp->PressPointerKey(EKeys::LeftMouseButton);
	}
}

void AMainPlayer::CheckWeaponSpeed()
{
	UMotionControllerComponent* Hand = nullptr;
	if (Weapon->GetAttachParentActor() == LeftHand->GetAttachmentRootActor())
	{
		Hand = LeftHand;
	}
	else if (Weapon->GetAttachParentActor() == RightHand->GetAttachmentRootActor())
	{
		Hand = RightHand;
	}

	if (Hand != nullptr)
	{
		// WeaponSpeed는 hand 의 속도를 가져온다.
		float WeaponSpeed = Hand->GetPhysicsLinearVelocity().Size();
		if (WeaponSpeed > AttackSpeedThreshold)
		{
			Weapon->Attack(WeaponSpeed);
		}
	}
}

void AMainPlayer::TryGrabLeft()
{
	// 중심점
	FVector Center = LeftHand->GetComponentLocation();

	// 	// 충돌한 물체를 기억할 배열 (백업)
	// 	TArray<FOverlapResult> HitObj;
	// 	FCollisionQueryParams params;
	// 	params.AddIgnoredActor(this);
	// 	params.AddIgnoredComponent(LeftHand);
	// 	bool bHit = GetWorld()->OverlapMultiByChannel(HitObj, Center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange), params);

		// 충돌한 물체를 기억할 배열
	TArray<FOverlapResult> HitObj;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(LeftHand);
	bool bHit = GetWorld()->OverlapMultiByChannel(HitObj, Center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange), params);
	if (bHit == false)
	{
		return;
	}
	// 가장 가까운 물체를 잡도록 하자
	// 가장 가까운 물체 인덱스
	int32 Closest = 0;
	for (int i = 0; i < HitObj.Num(); ++i)
	{
		// 1. 물리기능이 활성화 되어 있는 대상만 판단
		if (HitObj[i].GetComponent()->IsSimulatingPhysics() == false)
		{
			continue;
		}
		// 잡기에 성공했다
		IsGrabedLeft = true;
		// 2.. 현재 손과 가장 가까운 대상과 이번에 검출할 대상과 더 가까운 대상이 있다면		
		// 필요속성 : 현재 가장 가까운 대상과 손과의 거리
		float ClosestDist = FVector::Dist(HitObj[Closest].GetActor()->GetActorLocation(), Center);
		// 필요속성 : 이번에 검출할 대상과 손과의 거리
		float NextDist = FVector::Dist(HitObj[0].GetActor()->GetActorLocation(), Center);

		// 3. 만약 이번 대상이 현재 대상보다 가깝다면,
		if (NextDist < ClosestDist)
		{

			// 가장 가까운 대상으로 변경하기
			Closest = i;
		}
	}

	// 잡기에 성공했다면
	if (IsGrabedLeft)
	{
		// 물체 물리기능 비활성화
		GrabbedObject = HitObj[Closest].GetComponent();
		GrabbedObject->SetSimulatePhysics(false);
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 손에 붙여주자
		GrabbedObject->AttachToComponent(LeftHand, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AMainPlayer::TryGrabRight()
{
	if (IsRemoteGrab)
	{
		RemoteGrab();
		return;
	}
	// 중심점
	FVector Center = RightHand->GetComponentLocation();

	// 충돌한 물체를 기억할 배열
	TArray<FOverlapResult> HitObj;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(RightHand);
	bool bHit = GetWorld()->OverlapMultiByChannel(HitObj, Center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange), params);
	if (bHit == false)
	{
		return;
	}
	// 가장 가까운 물체를 잡도록 하자
	// 가장 가까운 물체 인덱스
	int32 Closest = 0;
	for (int i = 0; i < HitObj.Num(); ++i)
	{
		// 1. 물리기능이 활성화 되어 있는 대상만 판단
		if (HitObj[i].GetComponent()->IsSimulatingPhysics() == false)
		{
			continue;
		}
		// 잡기에 성공했다
		IsGrabedRight = true;
		// 2.. 현재 손과 가장 가까운 대상과 이번에 검출할 대상과 더 가까운 대상이 있다면		
		// 필요속성 : 현재 가장 가까운 대상과 손과의 거리
		float ClosestDist = FVector::Dist(HitObj[Closest].GetActor()->GetActorLocation(), Center);
		// 필요속성 : 이번에 검출할 대상과 손과의 거리
		float NextDist = FVector::Dist(HitObj[0].GetActor()->GetActorLocation(), Center);

		// 3. 만약 이번 대상이 현재 대상보다 가깝다면,
		if (NextDist < ClosestDist)
		{

			// 가장 가까운 대상으로 변경하기
			Closest = i;
		}
	}

	// 잡기에 성공했다면
	if (IsGrabedRight)
	{
		// 물체 물리기능 비활성화
		GrabbedObject = HitObj[Closest].GetComponent();
		GrabbedObject->SetSimulatePhysics(false);
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 손에 붙여주자
		GrabbedObject->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AMainPlayer::UnTryGrabLeft()
{	// 잡은 물체가 있다면
	if (IsGrabedLeft == false)
	{
		return;
	}

	// 1. 잡지않은 상태로 전환
	IsGrabedLeft = false;
	// 2. 손에서 떼어내기
	GrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 3. 물리기능 활성화
	GrabbedObject->SetSimulatePhysics(true);
	// 4. 충돌기능 활성화
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// 던지기
	GrabbedObject->AddForce(ThrowDirection * MyThrowPower * GrabbedObject->GetMass());

	// 회전 시키기
	// 각속도 = (1 / dt) * dTheta(특정 축 기준 변위 각도 Axis, angle)
	float Angle;
	FVector Axis;
	DeltaRotation.ToAxisAndAngle(Axis, Angle);
	float dt = GetWorld()->DeltaTimeSeconds;
	FVector AngularVelocity = (1.0f / dt) * Angle * Axis;
	GrabbedObject->SetPhysicsAngularVelocityInRadians(AngularVelocity * TorquePower, true);

	GrabbedObject = nullptr;
}

void AMainPlayer::UnTryGrabRight()
{
	if (IsGrabedRight == false)
	{
		return;
	}

	// 1. 잡지않은 상태로 전환
	IsGrabedRight = false;
	// 2. 손에서 떼어내기
	GrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 3. 물리기능 활성화
	GrabbedObject->SetSimulatePhysics(true);
	// 4. 충돌기능 활성화
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// 던지기
	GrabbedObject->AddForce(ThrowDirection * MyThrowPower * GrabbedObject->GetMass());

	// 회전 시키기
	// 각속도 = (1 / dt) * dTheta(특정 축 기준 변위 각도 Axis, angle)
	float Angle;
	FVector Axis;
	DeltaRotation.ToAxisAndAngle(Axis, Angle);
	float dt = GetWorld()->DeltaTimeSeconds;
	FVector AngularVelocity = (1.0f / dt) * Angle * Axis;
	GrabbedObject->SetPhysicsAngularVelocityInRadians(AngularVelocity * TorquePower, true);

	GrabbedObject = nullptr;
}

void AMainPlayer::Grabbing()
{
	// 잡은 물체가 있다면
	if (IsGrabedLeft && IsGrabedRight == false)
	{
		return;
	}
	else if (IsGrabedLeft == true && IsGrabedRight == false)
	{
		// 던질 방향 업데이트
		ThrowDirection = LeftHand->GetComponentLocation() - PrevPos;
		// 회전 방향 업데이트
		DeltaRotation = LeftHand->GetComponentQuat() * PrevRot.Inverse();
		// 이전 위치 업데이트
		PrevPos = LeftHand->GetComponentLocation();
		// 이전회전값 업데이트
		PrevRot = LeftHand->GetComponentQuat();
	}
	else if (IsGrabedRight == true && IsGrabedLeft == false)
	{
		// 던질 방향 업데이트
		ThrowDirection = RightHand->GetComponentLocation() - PrevPos;
		// 회전 방향 업데이트
		DeltaRotation = RightHand->GetComponentQuat() * PrevRot.Inverse();
		// 이전 위치 업데이트
		PrevPos = RightHand->GetComponentLocation();
		// 이전회전값 업데이트
		PrevRot = RightHand->GetComponentQuat();
	}
	else if(IsGrabedLeft && IsGrabedRight == true)
	{
		// 던질 방향 업데이트
		ThrowDirection = LeftHand->GetComponentLocation() - PrevPos;
		// 회전 방향 업데이트
		DeltaRotation = LeftHand->GetComponentQuat() * PrevRot.Inverse();
		// 이전 위치 업데이트
		PrevPos = LeftHand->GetComponentLocation();
		// 이전회전값 업데이트
		PrevRot = LeftHand->GetComponentQuat();
		// 던질 방향 업데이트
		ThrowDirection = RightHand->GetComponentLocation() - PrevPos;
		// 회전 방향 업데이트
		DeltaRotation = RightHand->GetComponentQuat() * PrevRot.Inverse();
		// 이전 위치 업데이트
		PrevPos = RightHand->GetComponentLocation();
		// 이전회전값 업데이트
		PrevRot = RightHand->GetComponentQuat();
	}
	// 쿼터니언 공식
	// Angle1 = Q1 , Angle2 = Q2
	// Angle1 + Angle2 = Q1 * Q2
	// -Angle1 = Q2.Inverse()
	// Angle2 - Angle1 = Q2 * Q1.Inverse()
}

void AMainPlayer::ReleaseUIInput()
{
	// UI 에 이벤트를 전달하고 싶다.
	if (WidgetInteractionComp)
	{
		//WidgetInteractionComp->PressPointerKey(FKey(FName("LeftMouseButton")));
		WidgetInteractionComp->ReleasePointerKey(EKeys::LeftMouseButton);
	}
}

void AMainPlayer::RemoteGrab()
{
	// 충돌체크(구충돌)
	// 충돌한 물체들 기록할 배열
	// 충돌 질의 작성
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	Param.AddIgnoredComponent(RightAim);
	FVector StartPos = RightAim->GetComponentLocation();
	FVector EndPos = StartPos + RightAim->GetForwardVector() * RemoteDistance;

	FHitResult HitInfo;
	bool bHit = GetWorld()->SweepSingleByChannel(HitInfo, StartPos, EndPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(RemoteRadius), Param);

	// 충돌이 되면 잡아당기기 애니메이션 실행
	if (bHit && HitInfo.GetComponent()->IsSimulatingPhysics())
	{
		// 잡았다
		IsGrabedRight = true;
		// 잡은 물체 할당
		GrabbedObject = HitInfo.GetComponent();
		// -> 물체 물리기능 비활성화
		GrabbedObject->SetSimulatePhysics(false);
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// -> 손에 붙여주자
		GrabbedObject->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);

		// 원거리 물체가 손으로 끌려오도록 처리
		GetWorld()->GetTimerManager().SetTimer(GrabTimer, FTimerDelegate::CreateLambda(
			[this]()->void
			{
				// 이동 중간에 사용자가 놔버리면
				if (GrabbedObject == nullptr)
				{
					GetWorld()->GetTimerManager().ClearTimer(GrabTimer);
					return;
				}
		//  물체가 -> 손 위치로 도착
		FVector Pos = GrabbedObject->GetComponentLocation();
		FVector TargetPos = RightHand->GetComponentLocation();
		Pos = FMath::Lerp<FVector>(Pos, TargetPos, RemoteMoveSpeed * GetWorld()->DeltaTimeSeconds);
		GrabbedObject->SetWorldLocation(Pos);

		float Distance = FVector::Dist(Pos, TargetPos);
		// 거의 가까워졌다면
		if (Distance < 10)
		{
			// 이동 중단하기
			GrabbedObject->SetWorldLocation(TargetPos);

			PrevPos = RightHand->GetComponentLocation();
			PrevRot = RightHand->GetComponentQuat();

			GetWorld()->GetTimerManager().ClearTimer(GrabTimer);
		}
			}
		), 0.02f, true);
	}
}

void AMainPlayer::DrawDebugRemoteGrab()
{
	// 시각화 켜져있는지 여부 확인, 원거리물체 잡기 활성화 여부
	if (bDrawDebugRemoteGrab == false || IsRemoteGrab == false)
	{
		return;
	}

	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	Param.AddIgnoredComponent(RightAim);
	FVector StartPos = RightAim->GetComponentLocation();
	FVector EndPos = StartPos + RightAim->GetForwardVector() * RemoteDistance;

	FHitResult HitInfo;
	bool bHit = GetWorld()->SweepSingleByChannel(HitInfo, StartPos, EndPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(RemoteRadius), Param);

	// 그리기
	DrawDebugSphere(GetWorld(), StartPos, RemoteRadius, 10, FColor::Yellow);
	if (bHit)
	{
		DrawDebugSphere(GetWorld(), HitInfo.Location, RemoteRadius, 10, FColor::Yellow);
	}
}

