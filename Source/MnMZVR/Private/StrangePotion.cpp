// Fill out your copyright notice in the Description page of Project Settings.


#include "StrangePotion.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AStrangePotion::AStrangePotion()
{
	//�޽�
	ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/LJC/Assets/Fantastic_Village_Pack/meshes/props/food/SM_PROP_bottle_02.SM_PROP_bottle_02'")));
	//�޽� ũ��
	ItemMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	// SphereComp ������
	SphereComp->SetSphereRadius(50.0f);
	//	ItemInvenCol->SetSphereRadius(45.0f);
}

void AStrangePotion::BeginPlay()
{
}
