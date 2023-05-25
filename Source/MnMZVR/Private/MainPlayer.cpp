// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "MeleeWeaponBase.h"
#include "WeaponInventory.h"
#include "Components/ChildActorComponent.h"
#include "ItemInventory.h"
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
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
	//PlayerCamera->bUsePawnControlRotation = true;

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
	// �޼� ���Ǿ��ݸ���
	LeftHandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandSphere"));
	LeftHandSphere->SetupAttachment(LeftHandMesh);
	LeftHandSphere->SetSphereRadius(3);
	LeftHandSphere->SetCollisionProfileName(TEXT("PlayerPreset"));
	LeftHandSphere->SetRelativeLocation(FVector(0.0f, 7.0f, 2.0f));
	// ���� �ݶ��̴� �±�
	LeftHandSphere->ComponentTags.Add(TEXT("LeftHandSphere"));

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
	// ������ ���Ǿ��ݸ���
	RightHandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandSphere"));
	RightHandSphere->SetupAttachment(RightHandMesh);
	RightHandSphere->SetSphereRadius(3);
	RightHandSphere->SetCollisionProfileName(TEXT("PlayerPreset"));
	RightHandSphere->SetRelativeLocation(FVector(0.0f, 7.0f, 2.0f));
	// ������ �ݶ��̴� �±�
	RightHandSphere->ComponentTags.Add(TEXT("RightHandSphere"));


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

	// ��Ʈ �޽�
	BeltMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeltMesh"));
	BeltMeshComp->SetupAttachment(PlayerCamera);
	// ��Ʈ �޽� �ε� �� �Ҵ�
	ConstructorHelpers::FObjectFinder<UStaticMesh> BeltMesh(TEXT("/Script/Engine.StaticMesh'/Game/KJY/3Dmodel/Player_Belt.Player_Belt'"));
	if (BeltMesh.Succeeded())
	{
		BeltMeshComp->SetStaticMesh(BeltMesh.Object);
		BeltMeshComp->SetRelativeLocation(FVector(0, 0, 0));
		BeltMeshComp->SetRelativeRotation(FRotator(0, -90, 0));
		BeltMeshComp->SetRelativeScale3D(FVector(1, 1, 1));

	}

	// ���Լհ���
	RightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightAim"));
	RightAim->SetupAttachment(RootComponent);
	RightAim->SetTrackingMotionSource(FName("RightAim"));

	// �������Լհ���
	LeftAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftAim"));
	LeftAim->SetupAttachment(RootComponent);
	LeftAim->SetTrackingMotionSource(FName("LeftAim"));

	WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComp"));
	WidgetInteractionComp->SetupAttachment(RightAim);

	ItemCheckSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCheckSphere"));
	ItemCheckSphere->SetupAttachment(RootComponent);
	ItemCheckSphere->SetSphereRadius(100);
	ItemCheckSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	ItemCheckSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemCheckSphere->SetRelativeLocation(FVector(0.0f, 0.0f, 45.0f));

	WeaponInven = CreateDefaultSubobject<AWeaponInventory>(TEXT("WeaponInven"));
	// ���� WeaponInven �� ���Ͽ� ���̱�
	//WeaponInven->AttachToComponent(BeltMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponInvenLeft"));
	// ����Ʈ���� ���۸� �̿��ؼ� AActor �� WeaponInven �� ã�Ƽ� �Ҵ�
//	ConstructorHelpers::FClassFinder<AWeaponInventory> WeaponInvenClass(TEXT("/Script/VR_Tutorial.WeaponInventory"));

	ItemInven = CreateDefaultSubobject<AItemInventory>(TEXT("ItemInven"));

	MinSwingSpeed = 300.0f;
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
// 		// �÷��̾ ���¹����� ������ �����Ѵ�.
// 		RightHand->SetRelativeLocation(FVector(0, 0, 0));
// 		// ���ӵ� ��ġ�ϰ� ����
// 		RightAim->SetRelativeLocation(FVector(0, 0, 0));

	}

	// �÷��̾� ü���� �÷��̾� �ƽ� ü�°� ����
	PlayerHP = PlayerMaxHP;
	AttachWeaponInventory();
	AttachItemInventory();
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	// HMD �� ����� ���� ������
// 	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
// 	{
// 		//  -> ���� ī�޶� ����� ��ġ�ϵ��� ����
// 		RightHand->SetRelativeRotation(PlayerCamera->GetRelativeRotation());
// 		// ���ӵ� ��ġ�ϰ� ����
// 		RightAim->SetRelativeRotation(PlayerCamera->GetRelativeRotation());
// 
// 	}

	// Grabbing
	Grabbing();

	if (GrabbedObject && IsWeapon == true)
	{
		FVector CurrentPosition = GrabbedObject->GetComponentLocation();
		FVector Velocity = (CurrentPosition - LastGrabbedObjectPosition) / GetWorld()->DeltaTimeSeconds;
		CurrentGrabbedObjectVelocity = Velocity.Size();
		LastGrabbedObjectPosition = CurrentPosition;

		//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, FString::Printf(TEXT("WeaponVelocity : %f"), CurrentGrabbedObjectVelocity), true, FVector2D(3.0f, 3.0f));

	}

	if (LeftGrabbedObject && IsWeapon == true)
	{
		FVector CurrentPosition = LeftGrabbedObject->GetComponentLocation();
		FVector Velocity = (CurrentPosition - LastGrabbedObjectPositionLeft) / GetWorld()->DeltaTimeSeconds;
		CurrentGrabbedObjectVelocity = Velocity.Size();
		LastGrabbedObjectPositionLeft = CurrentPosition;

		//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, FString::Printf(TEXT("WeaponVelocity : %f"), CurrentGrabbedObjectVelocity), true, FVector2D(3.0f, 3.0f));

	}

	if (ItemInven)
	{
		
		
	    FString text1 = ItemInven->GetAttachParentSocketName().ToString();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("parent : %s"), *text1), true, FVector2D(3.0f, 3.0f));
		

	}
	
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
		enhancedInputComponent->BindAction(IA_ItemInvenSetPosition, ETriggerEvent::Started, this, &AMainPlayer::ItemInventoryPositionReset);
		enhancedInputComponent->BindAction(IA_Getitem, ETriggerEvent::Triggered, this, &AMainPlayer::GetItem);
		enhancedInputComponent->BindAction(IA_Menu, ETriggerEvent::Triggered, this, &AMainPlayer::MenuOn);
	}
}

void AMainPlayer::Move(const FInputActionValue& Values)
{
	// ������� �Է¿����� �յ��¿�� �̵��ϰ� �ʹ�.
	// 1. ������� �Է¿� ����
	FVector2D Axis = Values.Get<FVector2D>();
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		AddMovementInput(GetActorForwardVector(), Axis.X);
		AddMovementInput(GetActorRightVector(), Axis.Y);
	}
	// ���� HMD �� ����Ǿ� �ִٸ�
	else
	{
		AddMovementInput(PlayerCamera->GetForwardVector(), Axis.X);
		AddMovementInput(PlayerCamera->GetRightVector(), Axis.Y);
	}
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

void AMainPlayer::AttachWeaponInventory()
{
	// Attach the WeaponInventory actor to the socket in the static mesh
	AWeaponInventory* WeaponInventory = GetWorld()->SpawnActor<AWeaponInventory>();
	if (WeaponInventory)
	{
		FName SocketName(TEXT("WeaponInvenLeft"));
		WeaponInven = WeaponInventory;
		WeaponInventory->AttachToComponent(BeltMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}
}

void AMainPlayer::AttachItemInventory()
{
	GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("attach")), true, FVector2D(3.0f, 3.0f));
	if (ItemInven)
	{
		FName SocketName(TEXT("ItemBag"));
		// �������� ������ġ���� ����Ʈ�� �ٿ��ش�.
		ItemInven->AttachToComponent(BeltMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		// �������� �����Ҷ� ȸ�������ش�.
		ItemInven->SetActorRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		ItemInven->SetActorRelativeLocation(FVector(0.0f, -10.0f, -20.0f));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, FString::Printf(TEXT("Spawnattach")), true, FVector2D(3.0f, 3.0f));
		ItemInven = GetWorld()->SpawnActor<AItemInventory>();
		FName SocketName(TEXT("ItemBag"));
		//ItemInven->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// �������� ������ġ���� ����Ʈ�� �ٿ��ش�.
		ItemInven->AttachToComponent(BeltMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		// �������� �����Ҷ� ȸ�������ش�.
		ItemInven->SetActorRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		ItemInven->SetActorRelativeLocation(FVector(0.0f, -10.0f, -20.0f));
	}
}

void AMainPlayer::ItemInventoryPositionReset()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("press x")), true, FVector2D(3.0f, 3.0f));
	if (ItemInven)
	{
		// �������κ��丮�� �տ� ������ �� ���¶�� , ��ư�� ���� �ٽ� ����ItemBag ���� �����ش�.
		if (ItemInven->bIsAttachedBagInLeftHand == true)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("reset position")), true, FVector2D(3.0f, 3.0f));
			ItemInven->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ItemInven->AttachToComponent(BeltMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("ItemBag")));
			ItemInven->bIsOverlapBagColLeftHand = false;
			ItemInven->bIsAttachedBagInLeftHand = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemInven is nullptr"));
	}


}

void AMainPlayer::TryGrabLeft()
{

	if (IsRemoteGrab)
	{
		LeftGrabOn = true;
		RemoteGrabLeft();
		return;
	}
	// �߽���
	FVector Center = LeftHandMesh->GetComponentLocation();

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
		LeftGrabbedObject = HitObj[Closest].GetComponent();
		LastGrabbedObjectPositionLeft = LeftGrabbedObject->GetComponentLocation();
		LeftGrabbedObject->SetSimulatePhysics(false);
		LeftGrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// �տ� �ٿ�����
		LeftGrabbedObject->AttachToComponent(LeftHandMesh, FAttachmentTransformRules::KeepWorldTransform);

	}

}

void AMainPlayer::TryGrabRight()
{
	if (IsRemoteGrab && !bIsOverlappedRight && !bIsOverlappedLeft)
	{
		RightGrabOn = true;
		RemoteGrab();
		return;
	}
	// �߽���
	FVector Center = RightHandMesh->GetComponentLocation();

	// �浹�� ��ü�� ����� �迭
	TArray<FOverlapResult> HitObj;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(RightHandMesh);
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
		HitObj[i].GetActor()->ActorHasTag("Weapon");
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
		LastGrabbedObjectPosition = GrabbedObject->GetComponentLocation();
		GrabbedObject->SetSimulatePhysics(false);
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// �տ� �ٿ�����
		GrabbedObject->AttachToComponent(RightHand, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AMainPlayer::TryGrabRightInven()
{
	// if RIghthand isgrabed == false, RightHandSphere is overlap weapon inventory
	if (IsGrabedRight == false)
	{
		return;
	}
}


void AMainPlayer::TryGrabLeftInven()
{

}

void AMainPlayer::UnTryGrabLeft()
{	// ���� ��ü�� �ִٸ�
	if (IsGrabedLeft == false)
	{
		return;
	}
	if (LeftGrabbedObject != nullptr)
	{
		LeftGrabOn = false;
		// 1. �������� ���·� ��ȯ
		IsGrabedLeft = false;
		// 2. �տ��� �����
		LeftGrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		// 3. ������� Ȱ��ȭ
		LeftGrabbedObject->SetSimulatePhysics(true);
		// 4. �浹��� Ȱ��ȭ
		LeftGrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// ������
		LeftGrabbedObject->AddForce(ThrowDirection * MyThrowPower * LeftGrabbedObject->GetMass());

		// ȸ�� ��Ű��
		// ���ӵ� = (1 / dt) * dTheta(Ư�� �� ���� ���� ���� Axis, angle)
		float Angle;
		FVector Axis;
		DeltaRotation.ToAxisAndAngle(Axis, Angle);
		float dt = GetWorld()->DeltaTimeSeconds;
		FVector AngularVelocity = (1.0f / dt) * Angle * Axis;
		LeftGrabbedObject->SetPhysicsAngularVelocityInRadians(AngularVelocity * TorquePower, true);
		LeftGrabbedObject = nullptr;

	}
}

void AMainPlayer::UnTryGrabRight()
{
	RightGrabOn = false;
	if (IsGrabedRight == false)
	{
		return;
	}

	if (GrabbedObject != nullptr)
	{
		// 1. �������� ���·� ��ȯ
		IsGrabedRight = false;
		RightHandMesh->SetVisibility(true);
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
	else if (IsGrabedLeft && IsGrabedRight == true)
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
	TArray<FOverlapResult> HitObj;
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	Param.AddIgnoredComponent(RightAim);
	Param.AddIgnoredComponent(RightHand);
	Param.AddIgnoredComponent(RightHandMesh);
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
		// ��ü ������� ��Ȱ��ȭ
		GrabbedObject = HitInfo.GetComponent();
		RightHandMesh->SetVisibility(false);
		Weapon = Cast<AMeleeWeaponBase>(GrabbedActorRight);
		// ���� ����� Weapon �̶��
		if (GrabbedActorRight == Weapon && Weapon != nullptr)
		{
			IsWeapon = true;
			//GrabbedObjectWithTongsRight = nullptr;
			GrabbedObject->K2_AttachToComponent(RightHandMesh, TEXT("HandRSocket"), EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, false);
			RightHandMesh->SetVisibility(false);
			GrabbedActorRight->SetActorEnableCollision(false);
			UE_LOG(LogTemp, Warning, TEXT("grab Weapon on Right"))
		}

		Weapon = Cast<AMeleeWeaponBase>(GrabbedObject->GetOwner());
		if (Weapon)
		{
			Weapon->AttachToComponent(RightHandMesh, FAttachmentTransformRules::KeepWorldTransform);
			IsWeapon = true;
		}
		// -> ��ü ������� ��Ȱ��ȭ
		LastGrabbedObjectPosition = GrabbedObject->GetComponentLocation();
		GrabbedObject->SetSimulatePhysics(false);
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GrabbedObject->AttachToComponent(RightHandMesh, FAttachmentTransformRules::KeepWorldTransform);

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
		FVector TargetPos = RightHandMesh->GetComponentLocation() + HandOffset;
		Pos = FMath::Lerp<FVector>(Pos, TargetPos, RemoteMoveSpeed * GetWorld()->DeltaTimeSeconds);
		GrabbedObject->SetWorldLocation(Pos);

		float Distance = FVector::Dist(Pos, TargetPos);
		// ���� ��������ٸ�
		if (Distance < 10)
		{
			// �̵� �ߴ��ϱ�
			GrabbedObject->SetWorldLocation(TargetPos);

			PrevPos = RightHandMesh->GetComponentLocation();
			PrevRot = RightHandMesh->GetComponentQuat();

			GetWorld()->GetTimerManager().ClearTimer(GrabTimer);
		}
			}
		), 0.02f, true);
	}
}

void AMainPlayer::RemoteGrabLeft()
{
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);
	Param.AddIgnoredComponent(LeftAim);
	FVector StartPos = LeftAim->GetComponentLocation();
	FVector EndPos = StartPos + LeftAim->GetForwardVector() * RemoteDistance;

	FHitResult HitInfo;
	bool bHit = GetWorld()->SweepSingleByChannel(HitInfo, StartPos, EndPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(RemoteRadius), Param);

	// �浹�� �Ǹ� ��ƴ��� �ִϸ��̼� ����
	if (bHit && HitInfo.GetComponent()->IsSimulatingPhysics())
	{
		// ��Ҵ�
		IsGrabedLeft = true;
		// ���� ��ü �Ҵ�
		LeftGrabbedObject = HitInfo.GetComponent();
		// -> ��ü ������� ��Ȱ��ȭ
		LastGrabbedObjectPositionLeft = LeftGrabbedObject->GetComponentLocation();
		LeftGrabbedObject->SetSimulatePhysics(false);
		LeftGrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Weapon = Cast<AMeleeWeaponBase>(LeftGrabbedObject->GetOwner());
		if (Weapon)
		{
			IsWeapon = true;
		}

		// -> �տ� �ٿ�����
		LeftGrabbedObject->AttachToComponent(LeftHandMesh, FAttachmentTransformRules::KeepWorldTransform);

		// ���Ÿ� ��ü�� ������ ���������� ó��
		GetWorld()->GetTimerManager().SetTimer(GrabTimer, FTimerDelegate::CreateLambda(
			[this]()->void
			{
				// �̵� �߰��� ����ڰ� ��������
				if (LeftGrabbedObject == nullptr)
				{
					GetWorld()->GetTimerManager().ClearTimer(GrabTimer);
					return;
				}
		//  ��ü�� -> �� ��ġ�� ����
		FVector Pos = LeftGrabbedObject->GetComponentLocation();
		// TargetPos �� �޼ո޽ÿ��ִ� ��ġ�� ���� Palm_r �� �ٴ´�.		
		FVector TargetPos = LeftHandMesh->GetComponentLocation() + HandOffset;
		Pos = FMath::Lerp<FVector>(Pos, TargetPos, RemoteMoveSpeed * GetWorld()->DeltaTimeSeconds);
		LeftGrabbedObject->SetWorldLocation(Pos);

		float Distance = FVector::Dist(Pos, TargetPos);
		// ���� ��������ٸ�
		if (Distance < 10)
		{
			// �̵� �ߴ��ϱ�
			LeftGrabbedObject->SetWorldLocation(TargetPos);

			PrevPos = LeftHandMesh->GetComponentLocation();
			PrevRot = LeftHandMesh->GetComponentQuat();

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
