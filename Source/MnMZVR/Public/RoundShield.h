// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoundShield.generated.h"

UCLASS()
class MNMZVR_API ARoundShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	// 방패메시
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ShieldMesh;
	// 공격이 됐는지 판정박스
	UPROPERTY(EditAnywhere)
	class UBoxComponent* ShieldBox;
	// 방패 데미지
	UPROPERTY(EditAnywhere)
	float ShieldDamage = 50.0f;

	UFUNCTION(BlueprintCallable, Category = "Shield")
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
};
