// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"

#include "CustomerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::CheckWaitzone()
{
// 	// 인스턴스 상에 있는 모든 대기존를 배열에 담는다
// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChair::StaticClass(), chairs);
// 
// 	// bIsSit 이라는 bool 배열에 대기존이 비어 있는지 아닌지를 담는다
}

void ASpawnManager::SpawnCustomer()
{
	// 시간이 지날때마다 손님을 스폰한다
	FTimerHandle SpawnTime;
	GetWorldTimerManager().SetTimer(SpawnTime, this, &ASpawnManager::SpawnCustom, 2, true);
}

void ASpawnManager::SpawnCustom()
{
// 	auto gameInstance = Cast<UBarGameInstance>(GetGameInstance());
// 
// 	// 날짜가 지난만큼 더 많이 스폰
// 	int32 idx = (gameInstance->checkDayCount - 1) * 4;
// 
// 	// 전체 손님 수가 지정한 숫자보다 작을때
// 	if (checkCustomerNum < idx)
// 	{
// 		for (int i = 0; i < bIsSit.Num(); i++)
// 		{
// 			// 빈자리가 있으면
// 			if (bIsSit[i] != true)
// 			{
// 				bCheckSit = true;
// 			}
// 		}
// 		// 손님을 스폰하고
// 		if (bCheckSit != false)
// 		{
// 			GetWorld()->SpawnActor<ACustomerCharacter>(customerFactory, GetActorLocation(), GetActorRotation());
// 
// 			// 한명씩 카운트하기
// 			checkCustomerNum++;
// 
// 			// 빈자리 채움
// 			bCheckSit = false;
// 
// 			// 가게 오픈했는지 체크
// 			bCheckSpawn = true;
// 		}
// 	}
}

