// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

UCLASS()
class MNMZVR_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 상점 스켈레탈 메시
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* ShopKeeperSkeletalMesh;
	// 상점 메시
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ShopMesh;


	// 상점 오버랩박스
	UPROPERTY(EditAnywhere)
	class UBoxComponent* ShopBoxComp;

	// 상점 오버랩
	UFUNCTION()
	void OnOverlapShop(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
