// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "MainPlayer.h"
#include "Enemy_Skeleton.h"
#include "EnemyAnim.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	// 월드에서 MainPlayer 타깃 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass());
	//MainPlayer 타입으로 캐스팅
	target = Cast<AMainPlayer>(actor);
	//소유 객체 가져오기
	me = Cast<AEnemy_Skeleton>(GetOwner());
	// UEnemyAnim* 할당
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	//GameMode = Cast<MnMZGameModeBase>(GetWorld()->GetAuthGameMode());

	// Ai 컨트롤
	//ai = Cast<AAIController>(me->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 내 본체의 EnemyAnim State에 내 State를 넣어주고 싶다.

	//me->enemyAnim->state = this->mState;

	switch (mState)
	{
	case EEnemystate::Idle:
		IdleState();
		break;
	case EEnemystate::Move:
		MoveState();
		break;
	case EEnemystate::Attack:
		AttackState();
		break;
	case EEnemystate::Damage:
		DamageState();
		break;
	case EEnemystate::Die:
		DieState();
		break;
	}
}

//대기상태
void UEnemyFSM::IdleState()
{
	// 1. 시간이 흘렀으니
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 만약 경과 시간이 대기 시간을 초과 했다면
	if (currentTime > idleDelayTime)
	{
		// 3. 이동 상태로 전환하고 싶다.
		mState = EEnemystate::Move;
		// 경과 시간 초기화
		currentTime = 0;
		// 애니메이션 상태 동기화
		anim->animState = mState;
	}
}
//이동상태
void UEnemyFSM::MoveState()
{
	// 1. 타깃 목적지가 필요하다.
	FVector destiantion = target->GetActorLocation();
	// 2. 방향이 필요하다.
	FVector dir = destiantion - me->GetActorLocation();
	// 3. 방향으로 이동하고 싶다.
	me->AddMovementInput(dir.GetSafeNormal());

	// 타깃과 가까워지면 공격 상태로 전환하고 싶다.
	// 1. 만약 거리가 공격범위 안에 들어오면
	if (dir.Size() < attackRange)
	{
		//2. 공격 상태로 전환하고 싶다.
		mState = EEnemystate::Attack;
		// 애니메이션 상태 동기화
		anim->animState = mState;
		// 공격 애니메이션 재생 활성화
		anim->bAttackPlay = true;
		// 공격 상태 전환시 대기시간이 바로 끝나도록 처리
		currentTime = attackDelayTime;
	}
}
//공격상태
void UEnemyFSM::AttackState()
{
	// 목표 : 일정 시간에 한 번씩 공격하고 싶다.
		// 1. 시간이 흘러야 한다.
		currentTime += GetWorld()->DeltaTimeSeconds;		
	// 2. 공격 시간이 됐으니까
	if (currentTime > attackDelayTime)
	{
		// 3. 공격하고 싶다.
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
		/*GameMode->ShowPlayerHitWidget();
		target->HP -= 5;
		if (target->HP <= 0)
		{
			target->HP = 0;
		}
		GameMode->main_UI->PrintPlayerHP();

		if (target->HP <= 0)
		{
			GameMode->ShowDefeatWidget();
		}*/

		// 경과 시간 초기화
		currentTime = 0;
		anim->bAttackPlay = true;
	}
	// 목표 : 타깃이 공격 범위를 벗아나면 상태를 이동으로 전환하고 싶다.
	// 1. 타깃과의 거리가 필요하다.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	// 2. 타깃과의 거리가 공격 범위를 벗어났으니까
	if (distance > attackRange)
	{
		// 3. 상태를 이동으로 전환하고 싶다.
		mState = EEnemystate::Move;
		// 애니메이션 상태 동기화
		anim->animState = mState;
	}
}
//피격상태
void UEnemyFSM::DamageState()
{
	// 1. 시간이 흘렀으니까
		currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 만약 경과 시간이 대기 시간을 초과했다면
	if (currentTime > damageDelayTime)
	{
		// 3. 대기 상태로 전황하고 싶다.
		mState = EEnemystate::Idle;
		// 경과 시간 초기화
		currentTime = 0;
		// 애니메이션 상태 동기화
		anim->animState = mState;
	}
}
//죽음상태
void UEnemyFSM::DieState()
{
	//// 죽어가는 애니메이션이 끝나지 않았다면 계속 쓰러지고 싶다.
	//if (anim->bDieDone == false)
	//{
	//	return;
	//}

	//me->Destroy();
}

void UEnemyFSM::OnDamageProcess(int32 damageValue)
{

	//me->Destroy();

	// 체력 감소
	//hp -= damageValue;
	hp--;
	// 만약 체력이 남아있다면
	if (hp > 0)
	{
		// 상태를 피격 상황으로 변환
		mState = EEnemystate::Damage;

		currentTime = 0;
		// 피격 애니메이션 재생
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), 0);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	// 그렇지 않다면
	else
	{

		//GameMode->SubtractCurrentEnemyLeft();
		//GameMode->CheckEnemyLeft();
		////GameMode->main_UI->PrintPlayerMoney();
		//GameMode->main_UI->PrintPlayerMoneyAnimation(target->money, target->money + 500);
		//target->money += 500;
		//GameMode->main_UI->PrintEarnMoney(500);

		////죽음 애니메이션 재생
		//anim->PlayDamageAnim(TEXT("die"));
		// 상태를 죽음으로 전환
		mState = EEnemystate::Die;

		//isdying = true;

	}
	//애니메이션 상태 동기화
	anim->animState = mState;
}

