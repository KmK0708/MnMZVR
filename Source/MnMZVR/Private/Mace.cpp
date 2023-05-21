// Fill out your copyright notice in the Description page of Project Settings.


#include "Mace.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AMace::AMace()
{
	// 공격력
	MeleeDamage = 1.0f;
	WeaponMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Mace.SM_Mace'")));
	// AttackBox 크기,위치설정
	AttackBox->SetBoxExtent(FVector(10.0f, 10.0f, 10.0f));
	AttackBox->SetRelativeLocation(FVector(0.0f, 0.0f, 65.0f));
	//Spherecol 사이즈
	SphereCol->SetSphereRadius(10.0f);
	// Spherecol 위치
	SphereCol->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

}

void AMace::BeginPlay()
{
}
