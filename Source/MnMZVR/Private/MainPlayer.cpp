// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Enhanced input ���
	auto PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC)
	{
		// LocalPlayer
		auto localPlayer = PC->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
		if (subSystem)
		{
			subSystem->AddMappingContext(IMC_Defaults, 0);
		}
	}
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent)
	{
		enhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMainPlayer::Move);
	}
}

void AMainPlayer::Move(const FInputActionValue& Values)
{
	// ������� �Է¿����� �յ��¿�� �̵��ϰ� �ʹ�.
	// 1. ������� �Է¿� ����
	FVector2D Axis = Values.Get<FVector2D>();
	// 2. �յ��¿��� ������ �ʿ�.
	FVector Dir(Axis.X, Axis.Y, 0);
	// 3. �̵��ϰ�ʹ�.
	// P = P0 + vt
	FVector P0 = GetActorLocation();
	FVector vt = Dir * PlayerSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	UE_LOG(LogTemp, Warning, TEXT("move"));
	SetActorLocation(P);
}

