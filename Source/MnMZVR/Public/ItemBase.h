// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class MNMZVR_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// �����۸޽�
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PropMesh;
	// ������ ���� �ݸ���
	UPROPERTY(EditAnywhere)
	class USphereComponent* ItemBoxColl;

	UPROPERTY()
	class ATestEnemy* TestEnemy;

	UPROPERTY()
	class AEnemy_Skeleton* SkelEnemy;

	// MainPlayer
	UPROPERTY()
	class AMainPlayer* MainPlayer;

		// ������
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
