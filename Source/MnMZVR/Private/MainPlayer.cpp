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
	// �޼� �ݸ����ڽ�
	LeftHandBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollision"));
	LeftHandBox->SetupAttachment(LeftHandMesh);
	// �ݸ����ڽ� ũ�� ����
	LeftHandBox->SetBoxExtent(FVector(4, 4, 1));
	// �ݸ����ڽ� ��ġ ����
	LeftHandBox->SetRelativeLocation(FVector(-0.4f, 4.5f, 0.22f));
	LeftHandBox->SetCollisionProfileName(TEXT("BlockAll"));
	LeftHandBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftHandBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftHandBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
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

	AttackSpeedThreshold = 1000.0f;
	Weapon = CreateDefaultSubobject<AMeleeWeaponBase>(TEXT("Weapon"));
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

	// �÷��̾� ü���� �÷��̾� �ƽ� ü�°� ����
	PlayerHP = PlayerMaxHP;

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
	// UI �� �̺�Ʈ�� �����ϰ� �ʹ�.
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
		// WeaponSpeed�� hand �� �ӵ��� �����´�.
		float WeaponSpeed = Hand->GetPhysicsLinearVelocity().Size();
		if (WeaponSpeed > AttackSpeedThreshold)
		{
			Weapon->Attack(WeaponSpeed);
		}
	}
}

void AMainPlayer::TryGrabLeft()
{
	// �߽���
	FVector Center = LeftHand->GetComponentLocation();

	// 	// �浹�� ��ü�� ����� �迭 (���)
	// 	TArray<FOverlapResult> HitObj;
	// 	FCollisionQueryParams params;
	// 	params.AddIgnoredActor(this);
	// 	params.AddIgnoredComponent(LeftHand);
	// 	bool bHit = GetWorld()->OverlapMultiByChannel(HitObj, Center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange), params);

		// �浹�� ��ü�� ����� �迭
	TArray<FOverlapResult> HitObj;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(LeftHand);
	bool bHit = GetWorld()->OverlapMultiByChannel(HitObj, Center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange), params);
	if (bHit == false)
	{
		return;
	}
	// ���� ����� ��ü�� �⵵�� ����
	// ���� ����� ��ü �ε���
	int32 Closest = 0;
	for (int i = 0; i < HitObj.Num(); ++i)
	{
		// 1. ��������� Ȱ��ȭ �Ǿ� �ִ� ��� �Ǵ�
		if (HitObj[i].GetComponent()->IsSimulatingPhysics() == false)
		{
			continue;
		}
		// ��⿡ �����ߴ�
		IsGrabedLeft = true;
		// 2.. ���� �հ� ���� ����� ���� �̹��� ������ ���� �� ����� ����� �ִٸ�		
		// �ʿ�Ӽ� : ���� ���� ����� ���� �հ��� �Ÿ�
		float ClosestDist = FVector::Dist(HitObj[Closest].GetActor()->GetActorLocation(), Center);
		// �ʿ�Ӽ� : �̹��� ������ ���� �հ��� �Ÿ�
		float NextDist = FVector::Dist(HitObj[0].GetActor()->GetActorLocation(), Center);

		// 3. ���� �̹� ����� ���� ��󺸴� �����ٸ�,
		if (NextDist < ClosestDist)
		{

			// ���� ����� ������� �����ϱ�
			Closest = i;
		}
	}

	// ��⿡ �����ߴٸ�
	if (IsGrabedLeft)
	{
		// ��ü ������� ��Ȱ��ȭ
		GrabbedObject = HitObj[Closest].GetComponent();
		GrabbedObject->SetSimulatePhysics(false);
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// �տ� �ٿ�����
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
	// �߽���
	FVector Center = RightHand->GetComponentLocation();

	// �浹�� ��ü�� ����� �迭
	TArray<FOverlapResult> HitObj;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(RightHand);
	bool bHit = GetWorld()->OverlapMultiByChannel(HitObj, Center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GrabRange), params);
	if (bHit == false)
	{
		return;
	}
	// ���� ����� ��ü�� �⵵�� ����
	// ���� ����� ��ü �ε���
	int32 Closest = 0;
	for (int i = 0; i < HitObj.Num(); ++i)
	{
		// 1. ��������� Ȱ��ȭ �Ǿ� �ִ� ��� �Ǵ�
		if (HitObj[i].GetComponent()->IsSimulatingPhysics() == false)
		{
			continue;
		}
		// ��⿡ �����ߴ�
		IsGrabedRight = true;
		// 2.. ���� �հ� ���� ����� ���� �̹��� ������ ���� �� ����� ����� �ִٸ�		
		// �ʿ�Ӽ� : ���� ���� ����� ���� �հ��� �Ÿ�
		float ClosestDist = FVector::Dist(HitObj[Closest].GetActor()->GetActorLocation(), Center);
		// �ʿ�Ӽ� : �̹��� ������ ���� �հ��� �Ÿ�
		float NextDist = FVector::Dist(HitObj[0].GetActor()->GetActorLocation(), Center);

		// 3. ���� �̹� ����� ���� ��󺸴� �����ٸ�,
		if (NextDist < ClosestDist)
		{

			// ���� ����� ������� �����ϱ�
			Closest = i;
		}
	}

	// ��⿡ �����ߴٸ�
	if (IsGrabedRight)
	{
		// ��ü ������� ��Ȱ��ȭ
		GrabbedObject = HitObj[Closest].GetComponent();
		GrabbedObject->SetSimulatePhysics(false);
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// �տ� �ٿ�����
		GrabbedObject->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AMainPlayer::UnTryGrabLeft()
{	// ���� ��ü�� �ִٸ�
	if (IsGrabedLeft == false)
	{
		return;
	}

	// 1. �������� ���·� ��ȯ
	IsGrabedLeft = false;
	// 2. �տ��� �����
	GrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 3. ������� Ȱ��ȭ
	GrabbedObject->SetSimulatePhysics(true);
	// 4. �浹��� Ȱ��ȭ
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// ������
	GrabbedObject->AddForce(ThrowDirection * MyThrowPower * GrabbedObject->GetMass());

	// ȸ�� ��Ű��
	// ���ӵ� = (1 / dt) * dTheta(Ư�� �� ���� ���� ���� Axis, angle)
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

	// 1. �������� ���·� ��ȯ
	IsGrabedRight = false;
	// 2. �տ��� �����
	GrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 3. ������� Ȱ��ȭ
	GrabbedObject->SetSimulatePhysics(true);
	// 4. �浹��� Ȱ��ȭ
	GrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// ������
	GrabbedObject->AddForce(ThrowDirection * MyThrowPower * GrabbedObject->GetMass());

	// ȸ�� ��Ű��
	// ���ӵ� = (1 / dt) * dTheta(Ư�� �� ���� ���� ���� Axis, angle)
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
	// ���� ��ü�� �ִٸ�
	if (IsGrabedLeft && IsGrabedRight == false)
	{
		return;
	}
	else if (IsGrabedLeft == true && IsGrabedRight == false)
	{
		// ���� ���� ������Ʈ
		ThrowDirection = LeftHand->GetComponentLocation() - PrevPos;
		// ȸ�� ���� ������Ʈ
		DeltaRotation = LeftHand->GetComponentQuat() * PrevRot.Inverse();
		// ���� ��ġ ������Ʈ
		PrevPos = LeftHand->GetComponentLocation();
		// ����ȸ���� ������Ʈ
		PrevRot = LeftHand->GetComponentQuat();
	}
	else if (IsGrabedRight == true && IsGrabedLeft == false)
	{
		// ���� ���� ������Ʈ
		ThrowDirection = RightHand->GetComponentLocation() - PrevPos;
		// ȸ�� ���� ������Ʈ
		DeltaRotation = RightHand->GetComponentQuat() * PrevRot.Inverse();
		// ���� ��ġ ������Ʈ
		PrevPos = RightHand->GetComponentLocation();
		// ����ȸ���� ������Ʈ
		PrevRot = RightHand->GetComponentQuat();
	}
	else if(IsGrabedLeft && IsGrabedRight == true)
	{
		// ���� ���� ������Ʈ
		ThrowDirection = LeftHand->GetComponentLocation() - PrevPos;
		// ȸ�� ���� ������Ʈ
		DeltaRotation = LeftHand->GetComponentQuat() * PrevRot.Inverse();
		// ���� ��ġ ������Ʈ
		PrevPos = LeftHand->GetComponentLocation();
		// ����ȸ���� ������Ʈ
		PrevRot = LeftHand->GetComponentQuat();
		// ���� ���� ������Ʈ
		ThrowDirection = RightHand->GetComponentLocation() - PrevPos;
		// ȸ�� ���� ������Ʈ
		DeltaRotation = RightHand->GetComponentQuat() * PrevRot.Inverse();
		// ���� ��ġ ������Ʈ
		PrevPos = RightHand->GetComponentLocation();
		// ����ȸ���� ������Ʈ
		PrevRot = RightHand->GetComponentQuat();
	}
	// ���ʹϾ� ����
	// Angle1 = Q1 , Angle2 = Q2
	// Angle1 + Angle2 = Q1 * Q2
	// -Angle1 = Q2.Inverse()
	// Angle2 - Angle1 = Q2 * Q1.Inverse()
}

void AMainPlayer::ReleaseUIInput()
{
	// UI �� �̺�Ʈ�� �����ϰ� �ʹ�.
	if (WidgetInteractionComp)
	{
		//WidgetInteractionComp->PressPointerKey(FKey(FName("LeftMouseButton")));
		WidgetInteractionComp->ReleasePointerKey(EKeys::LeftMouseButton);
	}
}

void AMainPlayer::RemoteGrab()
{
	// �浹üũ(���浹)
	// �浹�� ��ü�� ����� �迭
	// �浹 ���� �ۼ�
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	Param.AddIgnoredComponent(RightAim);
	FVector StartPos = RightAim->GetComponentLocation();
	FVector EndPos = StartPos + RightAim->GetForwardVector() * RemoteDistance;

	FHitResult HitInfo;
	bool bHit = GetWorld()->SweepSingleByChannel(HitInfo, StartPos, EndPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(RemoteRadius), Param);

	// �浹�� �Ǹ� ��ƴ��� �ִϸ��̼� ����
	if (bHit && HitInfo.GetComponent()->IsSimulatingPhysics())
	{
		// ��Ҵ�
		IsGrabedRight = true;
		// ���� ��ü �Ҵ�
		GrabbedObject = HitInfo.GetComponent();
		// -> ��ü ������� ��Ȱ��ȭ
		GrabbedObject->SetSimulatePhysics(false);
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// -> �տ� �ٿ�����
		GrabbedObject->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);

		// ���Ÿ� ��ü�� ������ ���������� ó��
		GetWorld()->GetTimerManager().SetTimer(GrabTimer, FTimerDelegate::CreateLambda(
			[this]()->void
			{
				// �̵� �߰��� ����ڰ� ��������
				if (GrabbedObject == nullptr)
				{
					GetWorld()->GetTimerManager().ClearTimer(GrabTimer);
					return;
				}
		//  ��ü�� -> �� ��ġ�� ����
		FVector Pos = GrabbedObject->GetComponentLocation();
		FVector TargetPos = RightHand->GetComponentLocation();
		Pos = FMath::Lerp<FVector>(Pos, TargetPos, RemoteMoveSpeed * GetWorld()->DeltaTimeSeconds);
		GrabbedObject->SetWorldLocation(Pos);

		float Distance = FVector::Dist(Pos, TargetPos);
		// ���� ��������ٸ�
		if (Distance < 10)
		{
			// �̵� �ߴ��ϱ�
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
	// �ð�ȭ �����ִ��� ���� Ȯ��, ���Ÿ���ü ��� Ȱ��ȭ ����
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

	// �׸���
	DrawDebugSphere(GetWorld(), StartPos, RemoteRadius, 10, FColor::Yellow);
	if (bHit)
	{
		DrawDebugSphere(GetWorld(), HitInfo.Location, RemoteRadius, 10, FColor::Yellow);
	}
}

