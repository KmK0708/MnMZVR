// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomerCharacter.h"

#include "CustomerAnimInstance.h"
#include "CustomerFSM.h"

// Sets default values
ACustomerCharacter::ACustomerCharacter()
{
//  	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;
// 
// 	// 여성 스켈레탈 메쉬
// 	ConstructorHelpers::FObjectFinder<USkeletalMesh> FemaleMesh1(TEXT("/Script/Engine.SkeletalMesh'/Game/Jang/Scanned3DPeoplePack/RP_Character/rp_carla_rigged_001_ue4/rp_carla_rigged_001_ue4.rp_carla_rigged_001_ue4'"));
// 	if (FemaleMesh1.Succeeded())
// 	{
// 		FemaleMesh.Add(FemaleMesh1);
// 	}
// 	ConstructorHelpers::FObjectFinder<USkeletalMesh> FemaleMesh2(TEXT("/Script/Engine.SkeletalMesh'/Game/Jang/Scanned3DPeoplePack/RP_Character/rp_claudia_rigged_002_ue4/rp_claudia_rigged_002_ue4.rp_claudia_rigged_002_ue4'"));
// 	if (FemaleMesh2.Succeeded())
// 	{
// 		FemaleMesh.Add(FemaleMesh2);
// 	}
// 	ConstructorHelpers::FObjectFinder<USkeletalMesh> FemaleMesh3(TEXT("/Script/Engine.SkeletalMesh'/Game/Jang/Scanned3DPeoplePack/RP_Character/rp_sophia_rigged_003_ue4/rp_sophia_rigged_003_ue4.rp_sophia_rigged_003_ue4'"));
// 	if (FemaleMesh3.Succeeded())
// 	{
// 		FemaleMesh.Add(FemaleMesh3);
// 	}
// 
// 	// 남성 스켈레탈 메쉬
// 	ConstructorHelpers::FObjectFinder<USkeletalMesh> MaleMesh1(TEXT("/Script/Engine.SkeletalMesh'/Game/Jang/Scanned3DPeoplePack/RP_Character/rp_eric_rigged_001_ue4/rp_eric_rigged_001_ue4.rp_eric_rigged_001_ue4'"));
// 	if (MaleMesh1.Succeeded())
// 	{
// 		MaleMesh.Add(MaleMesh1);
// 	}
// 	ConstructorHelpers::FObjectFinder<USkeletalMesh> MaleMesh2(TEXT("/Script/Engine.SkeletalMesh'/Game/Jang/Scanned3DPeoplePack/RP_Character/rp_manuel_rigged_001_ue4/rp_manuel_rigged_001_ue4.rp_manuel_rigged_001_ue4'"));
// 	if (MaleMesh2.Succeeded())
// 	{
// 		MaleMesh.Add(MaleMesh2);
// 	}
// 	ConstructorHelpers::FObjectFinder<USkeletalMesh> MaleMesh3(TEXT("/Script/Engine.SkeletalMesh'/Game/Jang/Scanned3DPeoplePack/RP_Character/rp_nathan_rigged_003_ue4/rp_nathan_rigged_003_ue4.rp_nathan_rigged_003_ue4'"));
// 	if (MaleMesh3.Succeeded())
// 	{
// 		MaleMesh.Add(MaleMesh3);
// 	}
// 
// 	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Jang/ABP_CustomerAnim.ABP_CustomerAnim_C'"));
// 	if (tempAnim.Succeeded())
// 	{
// 		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
// 	}
// 
// 	CustomerFSM = CreateDefaultSubobject<UCustomerFSM>(TEXT("CustomerFSM"));
// 
// // 	orderWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("orderWidget"));
// // 	orderWidget->SetupAttachment(GetMesh());
// 
// 	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ACustomerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CustomerAnim = Cast<UCustomerAnimInstance>(GetMesh()->GetAnimInstance());

//	order_UI = Cast<UCustomerOrderWidget>(orderWidget->GetUserWidgetObject());

	SetMesh();
}

// Called every frame
void ACustomerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACustomerCharacter::SetMesh()
{
 	int32 idx = CustomerFSM->SetRandRange(1, 10);
// 
// 	auto gm = Cast<A>(GetWorld()->GetAuthGameMode());
// 
// 	// 5보다 크면 여자
// 	if (idx > 5)
// 	{
// 		int32 womanIdx = customerFSM->SetRandRange(0, 2);
// 
// 		// 전에 나온 것과 똑같지 않으면
// 		if (womanIdx != gm->checkMeshCount)
// 		{
// 			GetMesh()->SetSkeletalMesh(FemaleMesh[womanIdx].Object);
// 
// 			if (womanIdx == 0 || womanIdx == 1)
// 			{
// 				GetMesh()->SetRelativeScale3D(FVector(1.1));
// 			}
// 
// 			gm->checkMeshCount = womanIdx;
// 		}
// 		// 똑같다면
// 		else
// 		{
// 			SetMesh();
// 		}
// 	}
// 	// 5보다 작거나 같으면 남자
// 	else
// 	{
// 		int32 manIdx = CustomerFSM->SetRandRange(0, 2);
// 
// 		// 전에 나온 것과 똑같지 않으면
// 		if (manIdx != gm->checkMeshCount)
// 		{
// 			GetMesh()->SetSkeletalMesh(MaleMesh[manIdx].Object);
// 
// 			gm->checkMeshCount = manIdx;
// 		}
// 		// 똑같다면
// 		else
// 		{
// 			SetMesh();
// 		}
// 	}
}

