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
// 	// �ν��Ͻ� �� �ִ� ��� ������� �迭�� ��´�
// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChair::StaticClass(), chairs);
// 
// 	// bIsSit �̶�� bool �迭�� ������� ��� �ִ��� �ƴ����� ��´�
}

void ASpawnManager::SpawnCustomer()
{
	// �ð��� ���������� �մ��� �����Ѵ�
	FTimerHandle SpawnTime;
	GetWorldTimerManager().SetTimer(SpawnTime, this, &ASpawnManager::SpawnCustom, 2, true);
}

void ASpawnManager::SpawnCustom()
{
// 	auto gameInstance = Cast<UBarGameInstance>(GetGameInstance());
// 
// 	// ��¥�� ������ŭ �� ���� ����
// 	int32 idx = (gameInstance->checkDayCount - 1) * 4;
// 
// 	// ��ü �մ� ���� ������ ���ں��� ������
// 	if (checkCustomerNum < idx)
// 	{
// 		for (int i = 0; i < bIsSit.Num(); i++)
// 		{
// 			// ���ڸ��� ������
// 			if (bIsSit[i] != true)
// 			{
// 				bCheckSit = true;
// 			}
// 		}
// 		// �մ��� �����ϰ�
// 		if (bCheckSit != false)
// 		{
// 			GetWorld()->SpawnActor<ACustomerCharacter>(customerFactory, GetActorLocation(), GetActorRotation());
// 
// 			// �Ѹ� ī��Ʈ�ϱ�
// 			checkCustomerNum++;
// 
// 			// ���ڸ� ä��
// 			bCheckSit = false;
// 
// 			// ���� �����ߴ��� üũ
// 			bCheckSpawn = true;
// 		}
// 	}
}

