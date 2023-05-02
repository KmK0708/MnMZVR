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

	// ���� �޽�
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(RootComponent);
	// ������� Ȱ��ȭ
	ShieldMesh->SetSimulatePhysics(true);
	// �޽� �ݸ��� ������ WeaponPreset
	ShieldMesh->SetCollisionProfileName(TEXT("WeaponPreset"));

	// AttackBox ����
	ShieldBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldBox"));
	ShieldBox->SetupAttachment(RootComponent);
	// ũ��
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
	// ���п� ���� ������ �¾�����

}

