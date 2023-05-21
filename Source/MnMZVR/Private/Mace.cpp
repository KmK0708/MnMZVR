// Fill out your copyright notice in the Description page of Project Settings.


#include "Mace.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AMace::AMace()
{
	// ���ݷ�
	MeleeDamage = 1.0f;
	WeaponMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Mace.SM_Mace'")));
	// AttackBox ũ��,��ġ����
	AttackBox->SetBoxExtent(FVector(10.0f, 10.0f, 10.0f));
	AttackBox->SetRelativeLocation(FVector(0.0f, 0.0f, 65.0f));
	//Spherecol ������
	SphereCol->SetSphereRadius(10.0f);
	// Spherecol ��ġ
	SphereCol->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

}

void AMace::BeginPlay()
{
}
