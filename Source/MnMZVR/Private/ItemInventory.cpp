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
	// �޽ü����ϱ�
	ConstructorHelpers::FObjectFinder<UStaticMesh> ItemBag(TEXT("/Script/Engine.StaticMesh'/Game/KJY/3Dmodel/BackPack/Static/BackPack.BackPack'"));
	if (ItemBag.Succeeded())
	{
		ItemBagMesh->SetStaticMesh(ItemBag.Object);
		ItemBagMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
		ItemBagMesh->SetRelativeLocation(FVector (0,0,0));
	}
	ItemBagCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemBagCollision"));
	ItemBagCollision->SetupAttachment(ItemBagMesh);
	ItemBagCollision->SetBoxExtent(FVector(70.f, 70.f, 150.f));
	ItemBagCollision->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	ItemSetUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSetUpCollision"));
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
		// �÷��̾��� ���� ���� �ݸ����� �������Ǿ����� true
		bIsOverlapBagColLeftHand = true;
		// �α� ����
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("LeftHandItemBag")), true, FVector2D(3.0f, 3.0f));
		
	}
	// �÷��̾��� ���̰� ���� LeftHandSphere �� ������ �Ǿ��� �޼��� �׷��� ��Ȱ��ȭ�� �����϶�.
	if (MainPlayer->IsGrabedLeft == false && bIsOverlapBagColLeftHand == true)
	{
		// �÷��̾��� �޼տ� �ٿ��ش�.
		// ũ�⸦ ���δ�.
		this->AttachToComponent(MainPlayer->LeftHandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HandLSocket"));
		// ȸ�������ش�.
		this->SetActorRelativeRotation(FRotator(90.f, 0.f, 0.f));
	}

}

void AItemInventory::OnItemSetUpOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADropItemBase* Item = Cast<ADropItemBase>(OtherActor);
	if (Item)
	{
		// ������ ����ġ
		Item->AttachToComponent(ItemSetUpCollision, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		MainPlayer->UnTryGrabRight();
		MainPlayer->IsGrabedRight = false;
		MainPlayer->RightGrabOn = false;
	}
}

