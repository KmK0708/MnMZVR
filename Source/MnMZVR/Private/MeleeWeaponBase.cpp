// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "MainPlayer.h"
#include "TestEnemy.h"
// 게임플레이스테틱
#include "Kismet/GameplayStatics.h"

// Sets default values
AMeleeWeaponBase::AMeleeWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	// 물리기능 활성화
	WeaponMesh->SetSimulatePhysics(true);
	// 메쉬 콜리전 프리셋 WeaponPreset
	WeaponMesh->SetCollisionProfileName(TEXT("WeaponPreset"));

	// AttackBox 생성
	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackBox->SetupAttachment(WeaponMesh);
	// AttackBox의 콜리전 프리셋 AttackBoxPreset
	AttackBox->SetCollisionProfileName(TEXT("AttackBoxPreset"));
	AttackBox->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap);
	AttackBox->SetGenerateOverlapEvents(false);
	// 어택박스 보이게하기
	AttackBox->SetHiddenInGame(false);

	// 무기 손잡이 오버랩콜리전
	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	SphereCol->SetupAttachment(WeaponMesh);
	SphereCol->SetCollisionProfileName(TEXT("WeaponHandOverlapPreset"));

	// player 캐스팅
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called when the game starts or when spawned
void AMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	prevPos = WeaponMesh->GetComponentLocation();
	// 공격이 됐는지 판정박스
	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlap);
	SphereCol->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlapHand);
	SphereCol->OnComponentEndOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlapEnd);
	if(!MainPlayer)MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

// Called every frame
void AMeleeWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WeaponVelocity = GetVelocity().Size();
	//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, FString::Printf(TEXT("WeaponVelocity : %f"), WeaponVelocity), true, FVector2D(3.0f, 3.0f));
}

void AMeleeWeaponBase::Attack()
{
	if (Enemy != nullptr)
	{
		
		float ModifiedDamage = MeleeDamage;
		Enemy->AddHealth(-ModifiedDamage);
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("Enemy Health : %f"), Enemy->EnemyHealth), true, FVector2D(3.0f, 3.0f));
		if (Enemy->EnemyHealth <= 0.f)
		{
			Enemy->Destroy();
			// Enemy is dead, handle death event
			// ...
		}
	}

}

void AMeleeWeaponBase::EndAttack()
{
	AttackBox->SetGenerateOverlapEvents(false); // 공격이 안됨
	Enemy = nullptr;
}

void AMeleeWeaponBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (AttackBox->IsOverlappingComponent(OtherComp))
	{
		ATestEnemy* TestEnemy = Cast<ATestEnemy>(OtherActor);
		if (TestEnemy != nullptr)
		{
			Enemy = TestEnemy;
			AttackBox->SetGenerateOverlapEvents(true); // 공격이 됨
			if (MainPlayer->IsGrabedLeft == true || MainPlayer->IsGrabedRight == true)
			{
				float WeaponSwingSpeed = MainPlayer->CurrentGrabbedObjectVelocity;
				//FVector Velocity = (CurrentPosition - MainPlayer->LastGrabbedObjectPosition) / GetWorld()->DeltaTimeSeconds;
				
				GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, FString::Printf(TEXT("WeaponVelocity : %f"), WeaponSwingSpeed), true, FVector2D(3.0f, 3.0f));
				if (WeaponSwingSpeed > 300)
				{
					// Weapon 의 어택함수
					Attack();
				}
				//MainPlayer->LastGrabbedObjectPosition = CurrentPosition;
			}
		}
	}
	

}

void AMeleeWeaponBase::OnOverlapHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	// 무기 손잡이 오버랩
// 	if (OverlappedComponent == SphereCol)
// 	{
// 		// 로그
// 		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("Overlap")), true, FVector2D(3.0f, 3.0f));
// 	}
 	UPrimitiveComponent * _rightHandSphere = Cast<UPrimitiveComponent>(MainPlayer->RightHandSphere);
 //	GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("Overlapped Somthing")), true, FVector2D(3.0f, 3.0f));
 	if (_rightHandSphere == OtherComp)
 	{
 		//if (OtherComp->ComponentHasTag("RightHandSphere"))
 		//{
 			bIsOverlapRight = true;
 			// 로그 띄우기
 			GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("RightHandOverlap")), true, FVector2D(3.0f, 3.0f));
 		//}
 	}
}

void AMeleeWeaponBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 플레이어의 오른쪽 구체가 오버랩이 끝났을때
	if (MainPlayer->RightHandSphere == nullptr)
	{
		bIsOverlapRight = false;
		// 로그 띄우기
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("RightHandOverlapEnd")), true, FVector2D(3.0f, 3.0f));
	}
}


float AMeleeWeaponBase::GetWeaponSpeed()
{
	FVector CurrentPos = WeaponMesh->GetComponentLocation();
	FVector Velocity = (CurrentPos - prevPos) / GetWorld()->DeltaTimeSeconds;
	float Speed = Velocity.Size();
	prevPos = CurrentPos;
	return Speed;
}

