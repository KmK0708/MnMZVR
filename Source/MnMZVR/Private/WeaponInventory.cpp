// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

// 박스오버랩
#include "Components/BoxComponent.h"
// 캐릭터
#include "MainPlayer.h"
// 무기베이스
#include "MeleeWeaponBase.h"


// Sets default values
AWeaponInventory::AWeaponInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponInventory::BeginPlay()
{
	Super::BeginPlay();
	

    // Add weapon overlap box to the root component of the character
    WeaponOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponOverlapBox"));
    WeaponOverlapBox->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    WeaponOverlapBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
    WeaponOverlapBox->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
    WeaponOverlapBox->SetCollisionProfileName(TEXT("WeaponOverlap"));

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
    AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
    if (Weapon && MainPlayer->IsGrabedLeft == true || MainPlayer->IsGrabedRight == true)
    {
        // Attach the weapon to the overlap box
        Weapon->AttachToComponent(OverlappedComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }
}

