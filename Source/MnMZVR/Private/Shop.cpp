// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainPlayer.h"

// Sets default values
AShop::AShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ShopKeeperSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShopKeeperSkeletalMesh"));
	ShopKeeperSkeletalMesh->SetupAttachment(RootComponent);
	// 스켈레탈 메시 로드 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJC/Assets/Sketchfab/Crowd-female/Peasant_Girl.Peasant_Girl'"));
	if (SK_Mesh.Succeeded())
	{
		ShopKeeperSkeletalMesh->SetSkeletalMesh(SK_Mesh.Object);
	}
	ShopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShopMesh"));
	ShopMesh->SetupAttachment(ShopKeeperSkeletalMesh);
	// 메시 로드 할당
	ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Script/Engine.StaticMesh'/Game/LJC/Assets/Fantastic_Village_Pack/meshes/props/furniture/SM_PROP_table_01.SM_PROP_table_01'"));
	if (Mesh.Succeeded())
	{
		ShopMesh->SetStaticMesh(Mesh.Object);
	}
	ShopBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ShopBoxComp"));
	ShopBoxComp->SetCollisionProfileName(TEXT("OverlapAll"));
	ShopBoxComp->SetupAttachment(ShopMesh);
	ShopBoxComp->SetRelativeLocation(FVector(0.0f, 100.0f, 50.0f));
	ShopBoxComp->SetBoxExtent(FVector(50.0f, 100.0f, 100.0f));

}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShop::OnOverlapShop(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

