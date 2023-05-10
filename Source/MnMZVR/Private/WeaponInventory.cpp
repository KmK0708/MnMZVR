// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

// �ڽ�������
#include "Components/BoxComponent.h"
// ĳ����
#include "MainPlayer.h"
// ���⺣�̽�
#include "MeleeWeaponBase.h"
#include <MotionControllerComponent.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponInventory::AWeaponInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    WeaponInvenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponInvenMesh"));
    WeaponInvenMesh->SetupAttachment(RootComponent);
//     // �޽ü����ϱ�
//     ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
// 
//     if (WeaponMesh.Succeeded())
//     {
// 		WeaponInvenMesh->SetStaticMesh(WeaponMesh.Object);
//         WeaponInvenMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
//         WeaponInvenMesh->SetRelativeLocation(FVector (0,0,0));
// 	}
	WeaponInvenMesh->SetupAttachment(RootComponent);
    WeaponInvenMesh->SetCollisionProfileName(TEXT("Inventory"));

    // Add weapon overlap box to the root component of the character
    WeaponOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponOverlapBox"));
    WeaponOverlapBox->SetupAttachment(WeaponInvenMesh);
    //   WeaponOverlapBox->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    WeaponOverlapBox->SetBoxExtent(FVector(12.f, 12.f, 12.f));
    WeaponOverlapBox->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
    // �������ڽ��� ���̰Լ���
    WeaponOverlapBox->SetHiddenInGame(false);
    WeaponOverlapBox->SetCollisionProfileName(TEXT("Inventory"));
}

// Called when the game starts or when spawned
void AWeaponInventory::BeginPlay()
{
	Super::BeginPlay();
	



    // Bind OnComponentBeginOverlap event
   WeaponOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponInventory::OnWeaponOverlapBegin);

}

// Called every frame
void AWeaponInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponInventory::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the overlapped actor is a weapon and if the weapon is being held by the player
    AMeleeWeaponBase* Weapon = Cast<AMeleeWeaponBase>(OtherActor);
    AMainPlayer* Mainplayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
    GrabActor = Weapon;

    // �������Ȱ� �÷��̾��� ���� �ݸ����ΰ� ������ �ݸ����ΰ� �Ǵ��ؾ���, ������ �÷��̾ �ݸ����� �������ִ°��� �Ǵ���.
    
    // ���� ����
    if (Weapon)
    {
        // ȭ�� �α����
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Weapon Overlap Begin"), true, FVector2D(3.0f, 3.0f));
       
        // �����տ� ����־��ٸ�.
        if (Mainplayer->IsGrabedRight == true && Mainplayer->IsWeapon == true)
        {
			// 1. ���� ���·� ��ȯ
			Mainplayer->IsGrabedRight = false;
			// 2.target is Weapon Parent is OverlappedComponent
			Weapon->AttachToComponent(WeaponOverlapBox, FAttachmentTransformRules::KeepRelativeTransform);
			// bool ���� ���� ����ġ�Ǿ������� true, �׷��� ������ false
            Mainplayer->IsWeapon = false;
            Mainplayer->RightHandMesh->SetVisibility(true);
            bIsWeaponAttached = true;
		}
        
        if (Mainplayer->IsGrabedLeft == true && Mainplayer->IsWeapon == true)
        {
            // Attach the weapon to the overlap box 
               // 1. �������� ���·� ��ȯ
            Mainplayer->IsGrabedLeft = false;
            // 2.target is Weapon Parent is OverlappedComponent
            Weapon->AttachToComponent(WeaponOverlapBox, FAttachmentTransformRules::KeepRelativeTransform);
            Mainplayer->IsWeapon = false;
            bIsWeaponAttached = true;
        }
        // bool ���� ���� ����ġ�Ǿ������� true, �׷��� ������ false
    }

    if (OtherComp->ComponentHasTag(TEXT("RightHandSphere")))
    {
        // The right hand sphere is overlapping
        // Perform actions for right hand overlap
        Mainplayer->bIsOverlappedRight = true;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RHand Overlap Begin"), true, FVector2D(3.0f, 3.0f));
        if (Mainplayer->IsGrabedRight == false && bIsWeaponAttached == true)
        {
            // ����ĳ��Ʈ�� ���.


            // ���Ⱑ �ִ��� Ȯ���Ѵ�.
            // ���Ⱑ �ִ°��� Ȯ�εǸ�
            // �κ��丮�� �����Ǿ��� ���⸦ ������ ��������.
        }
    }
    else if (OtherComp->ComponentHasTag(TEXT("LeftHandSphere")))
    {
        // The left hand sphere is overlapping
        // Perform actions for left hand overlap
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LHand Overlap Begin"), true, FVector2D(3.0f, 3.0f));
        if (Mainplayer->IsGrabedLeft == false && bIsWeaponAttached == true)
        {
            // �κ��丮�� �����Ǿ��� ���⸦ ������ ��������.

        }
    }

}

void AWeaponInventory::OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // �޼� ��ü �ݶ��̴��� �������� ������
    // 

}

