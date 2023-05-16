// Fill out your copyright notice in the Description page of Project Settings.


#include "HearthStone.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AHearthStone::AHearthStone()
{
	//�޽�
	ItemMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Dungeon/Rock/SM_Stone2.SM_Stone2'")));
	//�޽� ũ��
	ItemMesh->SetWorldScale3D(FVector(0.15f, 0.15f, 0.15f));

	// SphereComp ������
	SphereComp->SetSphereRadius(20.0f);
	ItemInvenCol->SetSphereRadius(16.0f);
}

void AHearthStone::BeginPlay()
{
}
