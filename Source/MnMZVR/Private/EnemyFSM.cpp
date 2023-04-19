// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

// Sets default values
AEnemyFSM::AEnemyFSM()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	// ���忡�� AVRPlayer Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass());
	//AVRPlayer Ÿ������ ĳ����
	target = Cast<AMainPlayer>(actor);
	//���� ��ü ��������
	me = Cast<AEnemy_Skeleton>(GetOwner());
	// UEnemyAnim* �Ҵ�
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	GameMode = Cast<AMnMZGameModeBase>(GetWorld()->GetAuthGameMode());

	// Ai ��Ʈ��
	ai = Cast<AAIController>(me->GetController());
}

// Called every frame
void AEnemyFSM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

