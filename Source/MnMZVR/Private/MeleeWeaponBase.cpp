// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "MainPlayer.h"
#include "TestEnemy.h"

// Sets default values
AMeleeWeaponBase::AMeleeWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	// ������� Ȱ��ȭ
	WeaponMesh->SetSimulatePhysics(true);
	// �޽� �ݸ��� ������ WeaponPreset
	WeaponMesh->SetCollisionProfileName(TEXT("WeaponPreset"));

	// AttackBox ����
	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackBox->SetupAttachment(RootComponent);
	// AttackBox�� �ݸ��� ������ AttackBoxPreset
	AttackBox->SetCollisionProfileName(TEXT("AttackBoxPreset"));

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
		// OnOverlap �Լ��� �����Ų��
		AttackBox->SetGenerateOverlapEvents(true);
	}
	else
	{
		// OnOverlap �Լ��� �����Ű�� �ʴ´�
		AttackBox->SetGenerateOverlapEvents(false);
	}
}

void AMeleeWeaponBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		ATestEnemy* TestEnemy = Cast<ATestEnemy>(OtherActor);
		if (TestEnemy != nullptr)
		{
			// Reduce the enemy's health
			if(WeaponMesh->GetPhysicsAngularVelocityInDegrees().Size() > 5.0)
			{
				TestEnemy->AddHealth(-MeleeDamage);
				GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, FString::Printf(TEXT("Enemy Health : %f"), TestEnemy->EnemyHealth), true, FVector2D(3.0f, 3.0f));
			}
			else 
			{
				TestEnemy->AddHealth(0);
				GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("Enemy Health : %f"), TestEnemy->EnemyHealth), true, FVector2D(3.0f, 3.0f));
			}
			// Check the print string to see if Enemy's health is declining
		}
	}
}

