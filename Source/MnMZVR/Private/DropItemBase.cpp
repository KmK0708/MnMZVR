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
// �����÷��̽���ƽ
#include "Kismet/GameplayStatics.h"

// Sets default values
ADropItemBase::ADropItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	// ������� Ȱ��ȭ
	ItemMesh->SetSimulatePhysics(true);
	// �޽� �ݸ��� ������ WeaponPreset
	ItemMesh->SetCollisionProfileName(TEXT("ItemPreset"));

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(ItemMesh);
	SphereComp->SetCollisionProfileName(TEXT("ItemOverlapPreset"));

	ItemInvenCol = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSphereCol"));
	ItemInvenCol->SetupAttachment(ItemMesh);
	ItemInvenCol->SetCollisionProfileName(TEXT("ItemOverlapPreset"));

	// player ĳ����
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called when the game starts or when spawned
void ADropItemBase::BeginPlay()
{
	Super::BeginPlay();
	ItemInvenCol->OnComponentBeginOverlap.AddDynamic(this, &ADropItemBase::OnOverlapInven);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ADropItemBase::OnOverlapHand);
	
	if (!MainPlayer)MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ADropItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADropItemBase::OnOverlapInven(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �κ��丮�� �������Ǹ� ����

}

void ADropItemBase::OnOverlapHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());

	UPrimitiveComponent* _rightHandSphere = Cast<UPrimitiveComponent>(MainPlayer->RightHandSphere);
	//	GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("Overlapped Somthing")), true, FVector2D(3.0f, 3.0f));
	if (_rightHandSphere == OtherComp)
	{
		bIsOverlapRightHand = true;
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("RightHandOverlap")), true, FVector2D(3.0f, 3.0f));
	}

	if (MainPlayer->bIsRightHandinWeaponInven == true && bIsOverlapRightHand == true && MainPlayer->IsGrabedRight == false)	// �����÷��̾�->bool ���� �������κ��� �ִ°� true
	{
		if (MainPlayer->WeaponInven->bIsWeaponAttached == true)	// �����÷��̾�->�������κ�-> bisItemAttached Ʈ��
		{
			if (MainPlayer->RightGrabOn == true)
			{
				// ������ Ű��
				ItemMesh->SetSimulatePhysics(false);
				MainPlayer->IsGrabedRight = true;
				// Attach yourself from your inventory to your hand.
				this->AttachToComponent(MainPlayer->RightHandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HandRSocket"));

			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("ExceptionPoint11")), true, FVector2D(3.0f, 3.0f));
	}
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

