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
	// ������� Ȱ��ȭ
	WeaponMesh->SetSimulatePhysics(true);

	// AttackBox ����
	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackBox->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	// ������ �ƴ��� �����ڽ�
	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlap);
}

// Called every frame
void AMeleeWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeleeWeaponBase::Attack()
{
	// ��� ��Ʈ�ѷ� �ֵθ��� �ӵ��� ���� ���ݱ�� Ȱ��ȭ
	if (WeaponMesh->GetPhysicsAngularVelocityInDegrees().Size() > 1000.0f)
	{

	}
}

void AMeleeWeaponBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

