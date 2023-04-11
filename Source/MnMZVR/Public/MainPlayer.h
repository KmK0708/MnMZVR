// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MainPlayer.generated.h"

UCLASS()
class MNMZVR_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// �÷��̾� �̵��ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	float PlayerSpeed = 600.0f;
	// �÷��̾� ��ǲ �������ؽ�Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* IMC_Defaults;
	// ��ǲ �׼�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_Move;

	// �̵�ó���Լ�
	void Move(const FInputActionValue& Values);
};
