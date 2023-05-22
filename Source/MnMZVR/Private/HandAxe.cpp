// Fill out your copyright notice in the Description page of Project Settings.


#include "HandAxe.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AHandAxe::AHandAxe()
{
	// ���ݷ�
	MeleeDamage = 2.0f;
	WeaponMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Axe.SM_Axe'")));
	// AttackBox ũ��,��ġ����
	AttackBox->SetBoxExtent(FVector(4.0f, 2.0f, 14.0f));
	AttackBox->SetRelativeLocation(FVector(14.0f, 0.0f, 45.0f));
	//Spherecol ������
	SphereCol->SetSphereRadius(20.0f);
	// Spherecol ��ġ
	SphereCol->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

void AHandAxe::BeginPlay()
{
}
