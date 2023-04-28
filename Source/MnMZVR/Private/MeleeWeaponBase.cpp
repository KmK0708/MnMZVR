// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
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
	AttackBox->SetupAttachment(RootComponent);
	// AttackBox의 콜리전 프리셋 AttackBoxPreset
	AttackBox->SetCollisionProfileName(TEXT("AttackBoxPreset"));
	AttackBox->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap);
	AttackBox->SetGenerateOverlapEvents(false);
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
				if (WeaponSwingSpeed > 100)
				{
					// Weapon 의 어택함수
					Attack();
				}
				//MainPlayer->LastGrabbedObjectPosition = CurrentPosition;
			}
		}
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

