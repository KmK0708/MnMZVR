// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMeleeWeaponBase::AMeleeWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	// 물리기능 활성화
	WeaponMesh->SetSimulatePhysics(true);

	// AttackBox 생성
	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackBox->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 공격이 됐는지 판정박스
	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlap);
}

// Called every frame
void AMeleeWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeleeWeaponBase::Attack()
{
	// 모션 컨트롤러 휘두르는 속도에 따른 공격기능 활성화
	if (WeaponMesh->GetPhysicsAngularVelocityInDegrees().Size() > 1000.0f)
	{

	}
}

void AMeleeWeaponBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

