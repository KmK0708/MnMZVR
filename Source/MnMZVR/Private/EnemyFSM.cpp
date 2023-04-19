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
	// 월드에서 AVRPlayer 타깃 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass());
	//AVRPlayer 타입으로 캐스팅
	target = Cast<AMainPlayer>(actor);
	//소유 객체 가져오기
	me = Cast<AEnemy_Skeleton>(GetOwner());
	// UEnemyAnim* 할당
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	GameMode = Cast<AMnMZGameModeBase>(GetWorld()->GetAuthGameMode());

	// Ai 컨트롤
	ai = Cast<AAIController>(me->GetController());
}

// Called every frame
void AEnemyFSM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

