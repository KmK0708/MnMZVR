  // Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "MainPlayer.h"
#include "WeaponInventory.h"
#include "TestEnemy.h"
#include "Enemy_Skeleton.h"
#include "EnemyFSM.h"
// �����÷��̽���ƽ
#include "Kismet/GameplayStatics.h"

// Sets default values
AMeleeWeaponBase::AMeleeWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	// ������� Ȱ��ȭ
	WeaponMesh->SetSimulatePhysics(true);
	// �޽� �ݸ��� ������ WeaponPreset


	// AttackBox ����
	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	AttackBox->SetupAttachment(WeaponMesh);
	// AttackBox�� �ݸ��� ������ AttackBoxPreset
	AttackBox->SetCollisionProfileName(TEXT("AttackBoxPreset"));
	AttackBox->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap);
	AttackBox->SetGenerateOverlapEvents(false);
	// ���ùڽ� ���̰��ϱ�
	AttackBox->SetHiddenInGame(false);

	// ���� ������ �������ݸ���
	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	SphereCol->SetupAttachment(WeaponMesh);
	SphereCol->SetCollisionProfileName(TEXT("WeaponHandOverlapPreset"));

	// player ĳ����
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called when the game starts or when spawned
void AMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	prevPos = WeaponMesh->GetComponentLocation();
	// ������ �ƴ��� �����ڽ�
	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlap);
	SphereCol->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlapHand);
	SphereCol->OnComponentEndOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlapEnd);
	if (!MainPlayer)MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	WeaponMesh->SetCollisionProfileName(TEXT("WeaponPreset"));
}

// Called every frame
void AMeleeWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WeaponVelocity = MainPlayer->HandVelocity;
	//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, FString::Printf(TEXT("WeaponVelocity : %f"), WeaponVelocity), true, FVector2D(3.0f, 3.0f));
}

void AMeleeWeaponBase::Attack()
{
	if (SkelEnemy != nullptr && bIsOverlapRight == true)
	{
		FSMEnemy = Cast<UEnemyFSM>(SkelEnemy->GetDefaultSubobjectByName(TEXT("fsm")));
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, FString::Printf(TEXT("EnemyTouch")), true, FVector2D(3.0f, 3.0f));
		if (FSMEnemy)
		{
			if (FSMEnemy->hp > 0)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, FString::Printf(TEXT("p1")), true, FVector2D(3.0f, 3.0f));
				FSMEnemy->OnDamageProcess(MeleeDamage);
				

			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, FString::Printf(TEXT("p2")), true, FVector2D(3.0f, 3.0f));
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, FString::Printf(TEXT("NullException")), true, FVector2D(3.0f, 3.0f));
		}

	}

	if (Enemy != nullptr)
	{

		float ModifiedDamage = MeleeDamage;
		Enemy->AddHealth(-ModifiedDamage);
		//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("Enemy Health : %f"), Enemy->EnemyHealth), true, FVector2D(3.0f, 3.0f));
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
	AttackBox->SetGenerateOverlapEvents(false); // ������ �ȵ�
	Enemy = nullptr;
}

void AMeleeWeaponBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (AttackBox->IsOverlappingComponent(OtherComp))
	{
		ATestEnemy* TestEnemy = Cast<ATestEnemy>(OtherActor);
		SkelEnemy = Cast<AEnemy_Skeleton>(OtherActor);
		if (SkelEnemy != nullptr)	//
		{
			AttackBox->SetGenerateOverlapEvents(true); // ������ ��
		
			float WeaponSwingSpeed = MainPlayer->HandVelocity;
			//FVector Velocity = (CurrentPosition - MainPlayer->LastGrabbedObjectPosition) / GetWorld()->DeltaTimeSeconds;

			//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, FString::Printf(TEXT("WeaponVelocity : %f"), WeaponSwingSpeed), true, FVector2D(3.0f, 3.0f));
			if (WeaponVelocity > 300 && bIsOverlapRight == true)
			{
				// Weapon �� �����Լ�
				Attack();
				// �ֵθ��� SwingSound ����ť ���
				UGameplayStatics::PlaySound2D(GetWorld(), SwingSound, 3.0f, 1.0f, 0.0f);
			}
			
		}
	}
}

void AMeleeWeaponBase::OnOverlapHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	// ���� ������ ������
// 	if (OverlappedComponent == SphereCol)
// 	{
// 		// �α�
// 		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("Overlap")), true, FVector2D(3.0f, 3.0f));
// 	}
	UPrimitiveComponent* _rightHandSphere = Cast<UPrimitiveComponent>(MainPlayer->RightHandSphere);
	//	GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("Overlapped Somthing")), true, FVector2D(3.0f, 3.0f));
	if (_rightHandSphere == OtherComp)
	{
		//if (OtherComp->ComponentHasTag("RightHandSphere"))
		//{
		bIsOverlapRight = true;
		// �α� ����
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("RightHandOverlap")), true, FVector2D(3.0f, 3.0f));
		//}
	}

	if (MainPlayer->bIsRightHandinWeaponInven == true && bIsOverlapRight == true && MainPlayer->IsGrabedRight == false)
	{
		if (MainPlayer->WeaponInven->bIsWeaponAttached == true)
		{
			if (MainPlayer->RightGrabOn == true)
			{
				// ������ Ű��
				//WeaponMesh->SetSimulatePhysics(false);
				MainPlayer->IsGrabedRight = true;
				MainPlayer->IsWeapon = true;
				// Attach yourself from your inventory to your hand.
				this->AttachToComponent(MainPlayer->RightHandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HandRSocket"));
				//WeaponMesh->AttachToComponent(MainPlayer->RightHandMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HandRSocket"));
				MainPlayer->WeaponInven->bIsWeaponAttached = false;

			}
		}
	}
	else
	{
		
	}
}

void AMeleeWeaponBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//MainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());

	UPrimitiveComponent* _rightHandSphere = Cast<UPrimitiveComponent>(MainPlayer->RightHandSphere);
	if (_rightHandSphere == OtherComp)
	{

		bIsOverlapRight = false;
		// �α� ����
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan, FString::Printf(TEXT("RightHandOverlapEnded")), true, FVector2D(3.0f, 3.0f));

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

