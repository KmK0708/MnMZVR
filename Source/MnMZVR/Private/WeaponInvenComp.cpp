// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInvenComp.h"

// 박스오버랩
#include "Components/BoxComponent.h"
// 캐릭터
#include "MainPlayer.h"
// 무기베이스
#include "MeleeWeaponBase.h"
#include "Components/SceneComponent.h"

// Sets default values for this component's properties
UWeaponInvenComp::UWeaponInvenComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWeaponInvenComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	

	AMainPlayer* Owner = Cast<AMainPlayer>(GetOwner());
	if (Owner)
	{
		USceneComponent* RootComponent = Owner->GetRootComponent();
		if (RootComponent)
		{
			WeaponOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponOverlapBox"));
			WeaponOverlapBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			WeaponOverlapBox->SetBoxExtent(FVector(10.0f, 10.0f, 2.0f));
			WeaponOverlapBox->SetRelativeLocation(FVector(0.f, 10.f, 10.f));
			WeaponOverlapBox->SetCollisionProfileName(TEXT("WeaponOverlap"));

			// Bind OnComponentBeginOverlap event
			WeaponOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &UWeaponInvenComp::OnWeaponOverlapBegin);
		}
	}
}


// Called every frame
void UWeaponInvenComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponInvenComp::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

