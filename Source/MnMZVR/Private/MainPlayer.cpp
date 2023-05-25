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
	// 카메라 설정
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetRelativeLocation(FVector(0, 0, 64));
	//PlayerCamera->bUsePawnControlRotation = true;

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
	// 왼손 스피어콜리전
	LeftHandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandSphere"));
	LeftHandSphere->SetupAttachment(LeftHandMesh);
	LeftHandSphere->SetSphereRadius(3);
	LeftHandSphere->SetCollisionProfileName(TEXT("PlayerPreset"));
	LeftHandSphere->SetRelativeLocation(FVector(0.0f, 7.0f, 2.0f));
	// 왼쪽 콜라이더 태그
	LeftHandSphere->ComponentTags.Add(TEXT("LeftHandSphere"));

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
	// 오른손 스피어콜리전
	RightHandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandSphere"));
	RightHandSphere->SetupAttachment(RightHandMesh);
	RightHandSphere->SetSphereRadius(3);
	RightHandSphere->SetCollisionProfileName(TEXT("PlayerPreset"));
	RightHandSphere->SetRelativeLocation(FVector(0.0f, 7.0f, 2.0f));
	// 오른쪽 콜라이더 태그
	RightHandSphere->ComponentTags.Add(TEXT("RightHandSphere"));


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

	// 벨트 메시
	BeltMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeltMesh"));
	BeltMeshComp->SetupAttachment(PlayerCamera);
	// 벨트 메시 로드 후 할당
	ConstructorHelpers::FObjectFinder<UStaticMesh> BeltMesh(TEXT("/Script/Engine.StaticMesh'/Game/KJY/3Dmodel/Player_Belt.Player_Belt'"));
	if (BeltMesh.Succeeded())
	{
		BeltMeshComp->SetStaticMesh(BeltMesh.Object);
		BeltMeshComp->SetRelativeLocation(FVector(0, 0, 0));
		BeltMeshComp->SetRelativeRotation(FRotator(0, -90, 0));
		BeltMeshComp->SetRelativeScale3D(FVector(1, 1, 1));

	}

	// 집게손가락
	RightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightAim"));
	RightAim->SetupAttachment(RootComponent);
	RightAim->SetTrackingMotionSource(FName("RightAim"));

	// 왼쪽집게손가락
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
	// 액터 WeaponInven 을 소켓에 붙이기
	//WeaponInven->AttachToComponent(BeltMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponInvenLeft"));
	// 컨스트럭터 헬퍼를 이용해서 AActor 인 WeaponInven 을 찾아서 할당
//	ConstructorHelpers::FClassFinder<AWeaponInventory> WeaponInvenClass(TEXT("/Script/VR_Tutorial.WeaponInventory"));

	ItemInven = CreateDefaultSubobject<AItemInventory>(TEXT("ItemInven"));

	MinSwingSpeed = 300.0f;
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
// 		// 플레이어가 보는방향을 앞으로 설정한다.
// 		RightHand->SetRelativeLocation(FVector(0, 0, 0));
// 		// 에임도 일치하게 하자
// 		RightAim->SetRelativeLocation(FVector(0, 0, 0));

	}

	// 플레이어 체력은 플레이어 맥스 체력과 동일
	PlayerHP = PlayerMaxHP;
	AttachWeaponInventory();
	AttachItemInventory();
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	// HMD 가 연결돼 있지 않으면
// 	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
// 	{
// 		//  -> 손이 카메라 방향과 일치하도록 하자
// 		RightHand->SetRelativeRotation(PlayerCamera->GetRelativeRotation());
// 		// 에임도 일치하게 하자
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
	// 사용자의 입력에따라 앞뒤좌우로 이동하고 싶다.
	// 1. 사용자의 입력에 따라
	FVector2D Axis = Values.Get<FVector2D>();
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() == false)
	{
		AddMovementInput(GetActorForwardVector(), Axis.X);
		AddMovementInput(GetActorRightVector(), Axis.Y);
	}
	// 만약 HMD 가 연결되어 있다면
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
	// UI 에 이벤트를 전달하고 싶다.
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
		// 아이템을 소켓위치에서 떨어트려 붙여준다.
		ItemInven->AttachToComponent(BeltMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		// 아이템을 스폰할때 회전시켜준다.
		ItemInven->SetActorRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		ItemInven->SetActorRelativeLocation(FVector(0.0f, -10.0f, -20.0f));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, FString::Printf(TEXT("Spawnattach")), true, FVector2D(3.0f, 3.0f));
		ItemInven = GetWorld()->SpawnActor<AItemInventory>();
		FName SocketName(TEXT("ItemBag"));
		//ItemInven->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// 아이템을 소켓위치에서 떨어트려 붙여준다.
		ItemInven->AttachToComponent(BeltMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		// 아이템을 스폰할때 회전시켜준다.
		ItemInven->SetActorRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		ItemInven->SetActorRelativeLocation(FVector(0.0f, -10.0f, -20.0f));
	}
}

void AMainPlayer::ItemInventoryPositionReset()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("press x")), true, FVector2D(3.0f, 3.0f));
	if (ItemInven)
	{
		// 아이템인벤토리가 손에 부착이 된 상태라면 , 버튼을 눌러 다시 소켓ItemBag 으로 돌려준다.
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
	// 중심점
	FVector Center = LeftHandMesh->GetComponentLocation();

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
		LeftGrabbedObject = HitObj[Closest].GetComponent();
		LastGrabbedObjectPositionLeft = LeftGrabbedObject->GetComponentLocation();
		LeftGrabbedObject->SetSimulatePhysics(false);
		LeftGrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 손에 붙여주자
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
	// 중심점
	FVector Center = RightHandMesh->GetComponentLocation();

	// 충돌한 물체를 기억할 배열
	TArray<FOverlapResult> HitObj;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredComponent(RightHandMesh);
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
		HitObj[i].GetActor()->ActorHasTag("Weapon");
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
		LastGrabbedObjectPosition = GrabbedObject->GetComponentLocation();
		GrabbedObject->SetSimulatePhysics(false);
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 손에 붙여주자
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
{	// 잡은 물체가 있다면
	if (IsGrabedLeft == false)
	{
		return;
	}
	if (LeftGrabbedObject != nullptr)
	{
		LeftGrabOn = false;
		// 1. 잡지않은 상태로 전환
		IsGrabedLeft = false;
		// 2. 손에서 떼어내기
		LeftGrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		// 3. 물리기능 활성화
		LeftGrabbedObject->SetSimulatePhysics(true);
		// 4. 충돌기능 활성화
		LeftGrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// 던지기
		LeftGrabbedObject->AddForce(ThrowDirection * MyThrowPower * LeftGrabbedObject->GetMass());

		// 회전 시키기
		// 각속도 = (1 / dt) * dTheta(특정 축 기준 변위 각도 Axis, angle)
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
		// 1. 잡지않은 상태로 전환
		IsGrabedRight = false;
		RightHandMesh->SetVisibility(true);
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
	else if (IsGrabedLeft && IsGrabedRight == true)
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

	// 충돌이 되면 잡아당기기 애니메이션 실행
	if (bHit && HitInfo.GetComponent()->IsSimulatingPhysics())
	{
		// 잡았다
		IsGrabedRight = true;
		// 잡은 물체 할당
		// 물체 물리기능 비활성화
		GrabbedObject = HitInfo.GetComponent();
		RightHandMesh->SetVisibility(false);
		Weapon = Cast<AMeleeWeaponBase>(GrabbedActorRight);
		// 잡은 대상이 Weapon 이라면
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
		// -> 물체 물리기능 비활성화
		LastGrabbedObjectPosition = GrabbedObject->GetComponentLocation();
		GrabbedObject->SetSimulatePhysics(false);
		GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GrabbedObject->AttachToComponent(RightHandMesh, FAttachmentTransformRules::KeepWorldTransform);

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
		FVector TargetPos = RightHandMesh->GetComponentLocation() + HandOffset;
		Pos = FMath::Lerp<FVector>(Pos, TargetPos, RemoteMoveSpeed * GetWorld()->DeltaTimeSeconds);
		GrabbedObject->SetWorldLocation(Pos);

		float Distance = FVector::Dist(Pos, TargetPos);
		// 거의 가까워졌다면
		if (Distance < 10)
		{
			// 이동 중단하기
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

	// 충돌이 되면 잡아당기기 애니메이션 실행
	if (bHit && HitInfo.GetComponent()->IsSimulatingPhysics())
	{
		// 잡았다
		IsGrabedLeft = true;
		// 잡은 물체 할당
		LeftGrabbedObject = HitInfo.GetComponent();
		// -> 물체 물리기능 비활성화
		LastGrabbedObjectPositionLeft = LeftGrabbedObject->GetComponentLocation();
		LeftGrabbedObject->SetSimulatePhysics(false);
		LeftGrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Weapon = Cast<AMeleeWeaponBase>(LeftGrabbedObject->GetOwner());
		if (Weapon)
		{
			IsWeapon = true;
		}

		// -> 손에 붙여주자
		LeftGrabbedObject->AttachToComponent(LeftHandMesh, FAttachmentTransformRules::KeepWorldTransform);

		// 원거리 물체가 손으로 끌려오도록 처리
		GetWorld()->GetTimerManager().SetTimer(GrabTimer, FTimerDelegate::CreateLambda(
			[this]()->void
			{
				// 이동 중간에 사용자가 놔버리면
				if (LeftGrabbedObject == nullptr)
				{
					GetWorld()->GetTimerManager().ClearTimer(GrabTimer);
					return;
				}
		//  물체가 -> 손 위치로 도착
		FVector Pos = LeftGrabbedObject->GetComponentLocation();
		// TargetPos 가 왼손메시에있는 위치와 소켓 Palm_r 에 붙는다.		
		FVector TargetPos = LeftHandMesh->GetComponentLocation() + HandOffset;
		Pos = FMath::Lerp<FVector>(Pos, TargetPos, RemoteMoveSpeed * GetWorld()->DeltaTimeSeconds);
		LeftGrabbedObject->SetWorldLocation(Pos);

		float Distance = FVector::Dist(Pos, TargetPos);
		// 거의 가까워졌다면
		if (Distance < 10)
		{
			// 이동 중단하기
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
