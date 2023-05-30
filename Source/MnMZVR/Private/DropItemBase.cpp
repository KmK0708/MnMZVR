// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemBase.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "MainPlayer.h"
#include "WeaponInventory.h"
#include "TestEnemy.h"
#include "Enemy_Skeleton.h"
#include "EnemyFSM.h"
#include "ItemInventory.h"
// �����÷��̽���ƽ
#include "Kismet/GameplayStatics.h"

// Sets default values
ADropItemBase::ADropItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
// 
// 	rootComp = CreateDefaultSubobject< UPrimitiveComponent>(TEXT("Root"));	
// 	RootComponent = rootComp;



	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);


	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp123"));
	SphereComp->SetupAttachment(ItemMesh);
	SphereComp->SetCollisionProfileName(TEXT("ItemPreset"));
	SphereComp->SetHiddenInGame(false);
	SphereComp->SetGenerateOverlapEvents(true);

//    ItemInvenCol = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSphereCol"));
//    ItemInvenCol->SetupAttachment(ItemMesh);
//    ItemInvenCol->SetCollisionProfileName(TEXT("ItemOverlapPreset"));
//    ItemInvenCol->SetHiddenInGame(false);
//    ItemInvenCol->SetCollisionResponseToChannel(ECC_GameTraceChannel10, ECR_Overlap);
//    ItemInvenCol->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// player ĳ����
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called when the game starts or when spawned
void ADropItemBase::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ADropItemBase::OnOverlapHand);
	//ItemInvenCol->OnComponentBeginOverlap.AddDynamic(this, &ADropItemBase::OnOverlapInven);

	if (!MainPlayer)MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ItemMesh->SetCollisionProfileName(TEXT("PropPreset"));
}

// Called every frame
void ADropItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADropItemBase::OnOverlapInven(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
// 	// �κ��丮�� �������Ǹ� ����
// 	if (ItemInventory->ItemSetUpCollision)
// 	{
// 		// ������ �� �ݸ��� ��ġ�� �ٷ� ����
// 		this->AttachToComponent(ItemInventory->ItemSetUpCollision, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
// 	}

}

void ADropItemBase::OnOverlapHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("please")), true, FVector2D(3.0f, 3.0f));

	UPrimitiveComponent* _rightHandSphere = Cast<UPrimitiveComponent>(MainPlayer->RightHandSphere);
	
	if (_rightHandSphere == OtherComp)
	{
		bIsOverlapRightHand = true;
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("RightHandOverlap")), true, FVector2D(3.0f, 3.0f));
	}
	
// 	if (MainPlayer && bIsOverlapRightHand == true && MainPlayer->IsGrabedRight == false)	// �����÷��̾�->bool ���� �������κ��� �ִ°� true
// 	{
// 		if (MainPlayer->ItemInven->bIsAttacheditem == true)	// �����÷��̾�->�������κ�-> bisItemAttached Ʈ��
// 		{
// 			if (MainPlayer->RightGrabOn == true)
// 			{
// 				// ������ Ű��
// 				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("PickItem")), true, FVector2D(3.0f, 3.0f));
// 				ItemMesh->SetSimulatePhysics(false);
// 				MainPlayer->IsGrabedRight = true;
// 				// Attach yourself from your inventory to your hand.
// 				this->AttachToComponent(MainPlayer->RightHandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HandRSocket"));
// 	
// 			}
// 		}
// 	}
// 	else
// 	{
// 		//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("ExceptionPoint11")), true, FVector2D(3.0f, 3.0f));
// 	}
}

void ADropItemBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	
	UPrimitiveComponent* _rightHandSphere = Cast<UPrimitiveComponent>(MainPlayer->RightHandSphere);
	if (_rightHandSphere == OtherComp)
	{
	
		bIsOverlapRightHand = false;
		// �α� ����
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("RightHandOverlapEnded")), true, FVector2D(3.0f, 3.0f));
	}

}

void ADropItemBase::SetPhysicsOff()
{
	ItemMesh->SetSimulatePhysics(false);
}

