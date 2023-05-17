// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventory.h"

#include "MainPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DropItemBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItemInventory::AItemInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemBagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemBagMesh"));
	ItemBagMesh->SetupAttachment(RootComponent);
	ItemBagMesh->SetCollisionProfileName(TEXT("AttachedPropPreset"));
	// 메시설정하기
	ConstructorHelpers::FObjectFinder<UStaticMesh> ItemBag(TEXT("/Script/Engine.StaticMesh'/Game/KJY/3Dmodel/BackPack/Static/BackPack.BackPack'"));
	if (ItemBag.Succeeded())
	{
		ItemBagMesh->SetStaticMesh(ItemBag.Object);
		ItemBagMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
		ItemBagMesh->SetRelativeLocation(FVector (0,0,0));
	}
	ItemBagCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemBagCollision"));
	ItemBagCollision->SetCollisionProfileName(TEXT("ItemBagPreset"));
	ItemBagCollision->SetupAttachment(ItemBagMesh);
	ItemBagCollision->SetBoxExtent(FVector(70.f, 70.f, 150.f));
	ItemBagCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	ItemSetUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSetUpCollision"));
	ItemSetUpCollision->SetCollisionProfileName(TEXT("ItemInventoryPreset"));
	ItemSetUpCollision->SetupAttachment(ItemBagMesh);
	ItemSetUpCollision->SetSphereRadius(20.f);
	ItemSetUpCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

}

// Called when the game starts or when spawned
void AItemInventory::BeginPlay()
{
	Super::BeginPlay();
	
	ItemBagCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemInventory::OnItemBagOverlap);
	ItemSetUpCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemInventory::OnItemSetUpOverlap);
	//SphereCol->OnComponentEndOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlapEnd);
	if (!MainPlayer)MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AItemInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemInventory::OnItemBagOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	UPrimitiveComponent* LeftHandSphere = Cast<UPrimitiveComponent>(MainPlayer->LeftHandSphere);

	if (LeftHandSphere == OtherComp)
	{
		// 플레이어의 손이 가방 콜리전에 오버랩되었으면 true
		bIsOverlapBagColLeftHand = true;
		// 로그 띄우기
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("LeftHandItemBag")), true, FVector2D(3.0f, 3.0f));
		
	}
	if (bIsAttachedBagInLeftHand == true)
	{
		return;
	}

	// 플레이어의 손이고 손의 LeftHandSphere 가 오버랩 되었고 왼손이 그랩이 비활성화된 상태일때.
	if (MainPlayer->IsGrabedLeft == false && bIsOverlapBagColLeftHand == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, FString::Printf(TEXT("LeftHandItemInven")), true, FVector2D(3.0f, 3.0f));

		// 플레이어의 왼손에 붙여준다.
		// 크기를 줄인다.
		MainPlayer->ItemInven->AttachToComponent(MainPlayer->LeftHandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HandLSocket"));
		// 회전시켜준다.
		MainPlayer->ItemInven->SetActorRelativeRotation(FRotator(0.f, 180.f, 0.f));
		bIsAttachedBagInLeftHand = true;
	}

}

void AItemInventory::OnItemSetUpOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADropItemBase* Item = Cast<ADropItemBase>(OtherActor);
	if (Item)
	{
		// 아이템 어태치
		bIsAttacheditem = true;
		Item->SetPhysicsOff();
		Item->AttachToComponent(ItemSetUpCollision, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		// 피직스 끈다.
		//MainPlayer->UnTryGrabRight();
		MainPlayer->IsGrabedRight = false;
		MainPlayer->RightGrabOn = false;
		MainPlayer->RightHandMesh->SetVisibility(true);
	}
}

