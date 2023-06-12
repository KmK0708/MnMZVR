// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainPlayer.h"
#include "Enemy_Skeleton.h"
#include "EnemyFSM.h"

// Sets default values
AEnemySpawn::AEnemySpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    // 박스콜리전
    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    BoxComp->SetCollisionProfileName(TEXT("OverlapAll"));
    BoxComp->SetupAttachment(RootComponent);
    // 크기설정
    BoxComp->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
    //콜리전을 올 오버랩으로
    BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    // 스폰 딜레이
    SpawnDelay = 1.0f;

    NumberOfEnemiesToSpawn = 4;

    Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();

    // Register the OnOverlapBegin() function to fire when this actor overlaps another actor
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawn::OnOverlapBegin);
}

// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult)
{
    // 닿은게 OtherActor 이고 OtherActor가 Player 라면
    if (OtherActor && (OtherActor != this) && OtherComp && (OtherActor->IsA(AMainPlayer::StaticClass())))
    {
		SpawnEnemies();
	}
}

void AEnemySpawn::SpawnEnemies()
{
    // 스폰 애너미
    
    
    for (int32 i = 0; i < NumberOfEnemiesToSpawn; i++)
    {
        FVector SpawnLocation = GetActorLocation(); // Customize as needed
        FRotator SpawnRotation = GetActorRotation(); // Customize as needed
        Enemy = GetWorld()->SpawnActor<AEnemy_Skeleton>(EnemyToSpawn, GetActorLocation(), FRotator::ZeroRotator);
    }

}

