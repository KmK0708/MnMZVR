// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MeleeWeaponBase.h"
#include "MainPlayer.h"

// Sets default values
ATestEnemy::ATestEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// enemy mesh
		// constructorhelpers�� �̿��ؼ� ĳ������ �޽��� �����´�.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempEnemyMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJC/Assets/UndeadPack/Ghoul/Mesh/SK_Ghoul_Full.SK_Ghoul_Full'"));
	// �ҷ����µ� �����ߴٸ�
	if (TempEnemyMesh.Succeeded())
	{
		// �޽��� GetMesh�� �����Ѵ�.
		GetMesh()->SetSkeletalMesh(TempEnemyMesh.Object);
		// transform�� �����Ѵ�.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
		// scale ����
		GetMesh()->SetRelativeScale3D(FVector(1.0f));
	}

}

void ATestEnemy::OnDeath()
{
	// ü���� 0���ϰ� �Ǹ�
	if (EnemyHealth <= 0)
	{
		// �׾��ٰ� ����
		bIsDead = true;
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		// ĸ��������Ʈ�� ��Ȱ��ȭ
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// �޽��� ��Ȱ��ȭ
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// �׾��ٰ� �˸�
		UE_LOG(LogTemp, Warning, TEXT("Enemy is Dead"));
	}
}

// Called when the game starts or when spawned
void ATestEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyHealth = EnemyMaxHealth;
}

// Called every frame
void ATestEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestEnemy::AddHealth(float Health)
{
	// ü���� ����
	float NewHeath = EnemyHealth + Health;
	EnemyHealth = NewHeath;
}
