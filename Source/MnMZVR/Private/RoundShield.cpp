// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundShield.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TestEnemy.h"
#include "MainPlayer.h"

// Sets default values
ARoundShield::ARoundShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 방패 메시
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(RootComponent);
	// 물리기능 활성화
	ShieldMesh->SetSimulatePhysics(true);
	// 메쉬 콜리전 프리셋 WeaponPreset
	ShieldMesh->SetCollisionProfileName(TEXT("WeaponPreset"));

	// AttackBox 생성
	ShieldBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldBox"));
	ShieldBox->SetupAttachment(RootComponent);
	// 크기
	ShieldBox->SetBoxExtent(FVector(20.0f, 20.0f, 4.0f));

}

// Called when the game starts or when spawned
void ARoundShield::BeginPlay()
{
	Super::BeginPlay();
	
	ShieldBox->OnComponentHit.AddDynamic(this, &ARoundShield::OnHit);
}

// Called every frame
void ARoundShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoundShield::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 방패에 적의 공격이 맞았을때

}

