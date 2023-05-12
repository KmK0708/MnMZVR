// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

// 박스오버랩
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
// 캐릭터
#include "MainPlayer.h"
// 무기베이스
#include "MeleeWeaponBase.h"
#include "LongSword.h"
#include <MotionControllerComponent.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponInventory::AWeaponInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    WeaponInvenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponInvenMesh"));
    WeaponInvenMesh->SetupAttachment(RootComponent);
//     // 메시설정하기
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
    // 오버랩박스를 보이게설정
    WeaponOverlapBox->SetHiddenInGame(false);
    WeaponOverlapBox->SetCollisionProfileName(TEXT("Inventory"));
}

// Called when the game starts or when spawned
void AWeaponInventory::BeginPlay()
{
	Super::BeginPlay();
	



    // Bind OnComponentBeginOverlap event
   WeaponOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponInventory::OnWeaponOverlapBegin);
   WeaponOverlapBox->OnComponentEndOverlap.AddDynamic(this, &AWeaponInventory::OnWeaponOverlapEnd);
}

// Called every frame
void AWeaponInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    CheckRaycast();
}

void AWeaponInventory::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the overlapped actor is a weapon and if the weapon is being held by the player
    AMeleeWeaponBase* Weapon = Cast<AMeleeWeaponBase>(OtherActor);
    AMainPlayer* Mainplayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
    // 오버랩된게 플레이어의 왼쪽 콜리전인가 오른쪽 콜리전인가 판단해야함, 지금은 플레이어가 콜리전은 가지고있는가를 판단중.
    
    // 무기 장착
    if (Weapon)
    {
        // 화면 로그찍기
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Weapon Overlap Begin"), true, FVector2D(3.0f, 3.0f));
       
        // 오른손에 쥐고있었다면.
        if (Mainplayer->IsGrabedRight == true && Mainplayer->IsWeapon == true)
        {
			// 1. 잡은 상태로 전환
			Mainplayer->IsGrabedRight = false;
			// 2.target is Weapon Parent is OverlappedComponent location rotation setting
            Weapon->AttachToComponent(WeaponOverlapBox, FAttachmentTransformRules:: KeepRelativeTransform);
            Mainplayer->IsWeapon = false;
            Mainplayer->RightHandMesh->SetVisibility(true);
            bIsWeaponAttached = true;
		}
        
        if (Mainplayer->IsGrabedLeft == true && Mainplayer->IsWeapon == true)
        {
            // Attach the weapon to the overlap box 
               // 1. 잡지않은 상태로 전환
            Mainplayer->IsGrabedLeft = false;
            // 2.target is Weapon Parent is OverlappedComponent
            Weapon->AttachToComponent(WeaponOverlapBox, FAttachmentTransformRules::KeepRelativeTransform);
   
            Mainplayer->IsWeapon = false;
            bIsWeaponAttached = true;
        }
        // bool 값을 만들어서 어태치되었을때는 true, 그랩을 누르면 false
    }

    if (OtherComp->ComponentHasTag(TEXT("RightHandSphere")))
    {
        Mainplayer->bIsRightHandinWeaponInven = true;
        if (Mainplayer->IsGrabedRight == false && bIsWeaponAttached == true)
        {
            Mainplayer->Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
    }
    else if (OtherComp->ComponentHasTag(TEXT("LeftHandSphere")))
    {
        Mainplayer->bIsLeftHandinWeaponInven = true;
        // The left hand sphere is overlapping
        // Perform actions for left hand overlap
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LHand Overlap Begin"), true, FVector2D(3.0f, 3.0f));
        if (Mainplayer->IsGrabedLeft == false && bIsWeaponAttached == true)
        {
            // 인벤토리에 부착되었던 무기를 손으로 가져간다.

        }
    }

}

void AWeaponInventory::OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Check if the overlapped actor is a weapon and if the weapon is being held by the player
    AMeleeWeaponBase* Weapon = Cast<AMeleeWeaponBase>(OtherActor);
    AMainPlayer* Mainplayer = Cast<AMainPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
    if (OtherComp->ComponentHasTag(TEXT("RightHandSphere")))
    {
        Mainplayer->bIsRightHandinWeaponInven = false;
        if (Mainplayer->IsGrabedRight == false && bIsWeaponAttached == true)
        {
            // 
        }
    }
    else if (OtherComp->ComponentHasTag(TEXT("LeftHandSphere")))
    {
        Mainplayer->bIsLeftHandinWeaponInven = false;

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LHand Overlap End"), true, FVector2D(3.0f, 3.0f));
        if (Mainplayer->IsGrabedLeft == false && bIsWeaponAttached == true)
        {
            // 인벤토리에 부착되었던 무기를 손으로 가져간다.

        }
    }
}

void AWeaponInventory::CheckRaycast()
{
    
}

