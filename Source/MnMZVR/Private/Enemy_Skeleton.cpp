// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Skeleton.h"

// Sets default values
AEnemy_Skeleton::AEnemy_Skeleton()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// ���̷�Ż �޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LJC/Assets/UndeadPack/SkeletonEnemy/Mesh/SK_Skeleton.SK_Skeleton'"));
	// 1.1 ������ �ε� �����ϸ�
	if (tempMesh.Succeeded())
	{
		//1.2 ������ �Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//1.3 �޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
		//1.4 �޽� ũ�� ����
		GetMesh()->SetRelativeScale3D(FVector(1.0f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("can't find mesh"));
	}
}

// Called when the game starts or when spawned
void AEnemy_Skeleton::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy_Skeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy_Skeleton::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

