// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomerCharacter.generated.h"

UCLASS()
class MNMZVR_API ACustomerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=FSM)
	class UCustomerFSM* CustomerFSM;
	UPROPERTY(EditAnywhere)
	class UCustomerAnimInstance* CustomerAnim;

	// 스폰시 메쉬 설정 함수
	void SetMesh();

	// 남성 메쉬
	TArray<ConstructorHelpers::FObjectFinder<USkeletalMesh>> MaleMesh;
	// 여성 메쉬
	TArray<ConstructorHelpers::FObjectFinder<USkeletalMesh>> FemaleMesh;
};
