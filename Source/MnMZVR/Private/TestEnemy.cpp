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
		// constructorhelpers를 이용해서 캐릭터의 메쉬를 가져온다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempEnemyMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJC/Assets/UndeadPack/Ghoul/Mesh/SK_Ghoul_Full.SK_Ghoul_Full'"));
	// 불러오는데 성공했다면
	if (TempEnemyMesh.Succeeded())
	{
		// 메쉬를 GetMesh로 적용한다.
		GetMesh()->SetSkeletalMesh(TempEnemyMesh.Object);
		// transform을 설정한다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
		// scale 수정
		GetMesh()->SetRelativeScale3D(FVector(1.0f));
	}

}

void ATestEnemy::OnDeath()
{
	// 체력이 0이하가 되면
	if (EnemyHealth <= 0)
	{
		// 죽었다고 설정
		bIsDead = true;
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		// 캡슐컴포넌트를 비활성화
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 메쉬를 비활성화
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 죽었다고 알림
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
	// 체력을 설정
	float NewHeath = EnemyHealth + Health;
	EnemyHealth = NewHeath;
}
