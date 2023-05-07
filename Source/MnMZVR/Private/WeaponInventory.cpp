// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventory.h"

// 박스오버랩
#include "Components/BoxComponent.h"
// 캐릭터
#include "MainPlayer.h"
// 무기베이스
#include "MeleeWeaponBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponInventory::AWeaponInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    WeaponInvenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponInvenMesh"));
    WeaponInvenMesh->SetupAttachment(RootComponent);
    // 메시설정하기
    ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

    if (WeaponMesh.Succeeded())
    {
		WeaponInvenMesh->SetStaticMesh(WeaponMesh.Object);
        WeaponInvenMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
        WeaponInvenMesh->SetRelativeLocation(FVector (0,0,0));
	}
	WeaponInvenMesh->SetupAttachment(RootComponent);
    WeaponInvenMesh->SetCollisionProfileName(TEXT("Inventory"));

    // Add weapon overlap box to the root component of the character
    WeaponOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponOverlapBox"));
    WeaponOverlapBox->SetupAttachment(WeaponInvenMesh);
    //   WeaponOverlapBox->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    WeaponOverlapBox->SetBoxExtent(FVector(20.f, 20.f, 20.f));
    WeaponOverlapBox->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
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

    if (Weapon)
    {
        // 화면 로그찍기
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Weapon Overlap Begin"), true, FVector2D(3.0f, 3.0f));
        

        // Attach the weapon to the overlap box 
        // 2.target is Weapon Parent is OverlappedComponent
        Weapon->AttachToComponent(WeaponOverlapBox, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

            // 1. 잡지않은 상태로 전환
        Mainplayer->IsGrabedRight = false;
        // 2. 손에서 떼어내기
        Mainplayer->GrabbedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        // 3. 물리기능 활성화
        Mainplayer->GrabbedObject->SetSimulatePhysics(true);

        // bool 값을 만들어서 어태치되었을때는 true, 그랩을 누르면 false
        
    }
}

