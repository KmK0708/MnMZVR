// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "WeaponInvenComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MNMZVR_API UWeaponInvenComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponInvenComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* WeaponOverlapBox;

    UFUNCTION()
    void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

// 	UFUNCTION()
// 	void OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 
// 	UFUNCTION()
// 	void AttachToComponent(USceneComponent* InParent, const FAttachmentTransformRules& InAttachmentRules, FName InSocketName);

};
