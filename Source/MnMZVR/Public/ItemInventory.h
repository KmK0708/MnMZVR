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

	// 가방이 손에 붙었는가
	bool bIsAttachedBagInLeftHand = false;

	// 아이템이 인벤토리 콜리전에 붙었는가.
	bool bIsAttacheditem = false;

	UFUNCTION()
	void OnItemBagOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnItemSetUpOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
