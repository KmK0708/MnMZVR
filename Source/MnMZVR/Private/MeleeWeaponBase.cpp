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
	WeaponMesh->SetupAttachment(RootComponent);
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

	prevPos = WeaponMesh->GetComponentLocation();
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
	float Weaponspeed = GetWeaponSpeed();


	if (Enemy != nullptr && WeaponSpeed > MinSpeedForDamage)
	{
		float DamageMultiplier = FMath::Clamp((WeaponSpeed - MinSpeedForDamage) / (MaxSpeedForDamage - MinSpeedForDamage), 0.0f, 1.0f);
		float DamageAmount = MeleeDamage * DamageMultiplier;
		Enemy->AddHealth(-DamageAmount);
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, FString::Printf(TEXT("Enemy Health : %f"), Enemy->EnemyHealth), true, FVector2D(3.0f, 3.0f));
	}

}

void AMeleeWeaponBase::EndAttack()
{
	AttackBox->SetGenerateOverlapEvents(false); // ������ �ȵ�
	Enemy = nullptr;
}

void AMeleeWeaponBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AttackBox->IsOverlappingComponent(OtherComp))
	{
		ATestEnemy* TestEnemy = Cast<ATestEnemy>(OtherActor);
		if (TestEnemy != nullptr)
		{
			Enemy = TestEnemy;
			AttackBox->SetGenerateOverlapEvents(true); // ������ ��
			Attack();
		}
	}
}


float AMeleeWeaponBase::GetWeaponSpeed()
{
	FVector CurrentPos = WeaponMesh->GetComponentLocation();
	FVector Velocity = (CurrentPos - prevPos) / GetWorld()->DeltaTimeSeconds;
	float Speed = Velocity.Size();
	prevPos = CurrentPos;
	return Speed;
}

