// Fill out your copyright notice in the Description page of Project Settings.


#include "LongSword.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ALongSword::ALongSword()
{
	// ���ݷ�
	MeleeDamage = 10.0f;
	WeaponMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Sword.SM_Sword'")));
	// AttackBox ũ��,��ġ����
	AttackBox->SetBoxExtent(FVector(4.0f, 4.0f, 51.0f)); 
	AttackBox->SetRelativeLocation(FVector(0.0f, 0.0f, -65.0f));
}

void ALongSword::BeginPlay()
{
	Super::BeginPlay();

	// Set unique collision profile for this sword
	WeaponMesh->SetCollisionProfileName(TEXT("WeaponPreset"));
}
