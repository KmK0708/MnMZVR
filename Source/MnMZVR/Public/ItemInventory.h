// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInventory.generated.h"

UCLASS()
class MNMZVR_API AItemInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ItemBagMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* ItemBagCollision;

	UPROPERTY(EditAnywhere)
	class USphereComponent* ItemSetUpCollision;

	// MainPlayer
	UPROPERTY(EditAnywhere)
	class AMainPlayer* MainPlayer;
	// Player Hand Overlap Bag Collision
	bool bIsOverlapBagColRightHand = false;
	bool bIsOverlapBagColLeftHand = false;

	// ������ �տ� �پ��°�
	bool bIsAttachedBagInLeftHand = false;

	// �������� �κ��丮 �ݸ����� �پ��°�.
	bool bIsAttacheditem = false;

	UFUNCTION()
	void OnItemBagOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnItemSetUpOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
