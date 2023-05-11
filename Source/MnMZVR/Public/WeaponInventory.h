// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponInventory.generated.h"

UCLASS()
class MNMZVR_API AWeaponInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* WeaponOverlapBox;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* WeaponInvenMesh;

	UPROPERTY(EditAnywhere)
	class AMainPlayer* Player;

	UPROPERTY(EditAnywhere)
	class AActor* GrabActor;

	UPROPERTY(EditAnywhere)
	float TraceDistance = 10.0f;

	UPROPERTY(EditAnywhere)
	bool bIsRaycastOn;

    UFUNCTION()
    void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void CheckRaycast();

	// 무기가 부착되었는가
	UPROPERTY(EditAnywhere)
	bool bIsWeaponAttached = false;
};
