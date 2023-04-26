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
	// ���忡�� MainPlayer Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass());
	//MainPlayer Ÿ������ ĳ����
	target = Cast<AMainPlayer>(actor);
	//���� ��ü ��������
	me = Cast<AEnemy_Skeleton>(GetOwner());
	// UEnemyAnim* �Ҵ�
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	//GameMode = Cast<MnMZGameModeBase>(GetWorld()->GetAuthGameMode());

	// Ai ��Ʈ��
	//ai = Cast<AAIController>(me->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �� ��ü�� EnemyAnim State�� �� State�� �־��ְ� �ʹ�.

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

//������
void UEnemyFSM::IdleState()
{
	// 1. �ð��� �귶����
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���� ��� �ð��� ��� �ð��� �ʰ� �ߴٸ�
	if (currentTime > idleDelayTime)
	{
		// 3. �̵� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemystate::Move;
		// ��� �ð� �ʱ�ȭ
		currentTime = 0;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	}
}
//�̵�����
void UEnemyFSM::MoveState()
{
	// 1. Ÿ�� �������� �ʿ��ϴ�.
	FVector destiantion = target->GetActorLocation();
	// 2. ������ �ʿ��ϴ�.
	FVector dir = destiantion - me->GetActorLocation();
	// 3. �������� �̵��ϰ� �ʹ�.
	me->AddMovementInput(dir.GetSafeNormal());

	// Ÿ��� ��������� ���� ���·� ��ȯ�ϰ� �ʹ�.
	// 1. ���� �Ÿ��� ���ݹ��� �ȿ� ������
	if (dir.Size() < attackRange)
	{
		//2. ���� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemystate::Attack;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		// ���� �ִϸ��̼� ��� Ȱ��ȭ
		anim->bAttackPlay = true;
		// ���� ���� ��ȯ�� ���ð��� �ٷ� �������� ó��
		currentTime = attackDelayTime;
	}
}
//���ݻ���
void UEnemyFSM::AttackState()
{
	// ��ǥ : ���� �ð��� �� ���� �����ϰ� �ʹ�.
		// 1. �ð��� �귯�� �Ѵ�.
		currentTime += GetWorld()->DeltaTimeSeconds;		
	// 2. ���� �ð��� �����ϱ�
	if (currentTime > attackDelayTime)
	{
		// 3. �����ϰ� �ʹ�.
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

		// ��� �ð� �ʱ�ȭ
		currentTime = 0;
		anim->bAttackPlay = true;
	}
	// ��ǥ : Ÿ���� ���� ������ ���Ƴ��� ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
	// 1. Ÿ����� �Ÿ��� �ʿ��ϴ�.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	// 2. Ÿ����� �Ÿ��� ���� ������ ������ϱ�
	if (distance > attackRange)
	{
		// 3. ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
		mState = EEnemystate::Move;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	}
}
//�ǰݻ���
void UEnemyFSM::DamageState()
{
	// 1. �ð��� �귶���ϱ�
		currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > damageDelayTime)
	{
		// 3. ��� ���·� ��Ȳ�ϰ� �ʹ�.
		mState = EEnemystate::Idle;
		// ��� �ð� �ʱ�ȭ
		currentTime = 0;
		// �ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	}
}
//��������
void UEnemyFSM::DieState()
{
	//// �׾�� �ִϸ��̼��� ������ �ʾҴٸ� ��� �������� �ʹ�.
	//if (anim->bDieDone == false)
	//{
	//	return;
	//}

	//me->Destroy();
}

void UEnemyFSM::OnDamageProcess(int32 damageValue)
{

	//me->Destroy();

	// ü�� ����
	//hp -= damageValue;
	hp--;
	// ���� ü���� �����ִٸ�
	if (hp > 0)
	{
		// ���¸� �ǰ� ��Ȳ���� ��ȯ
		mState = EEnemystate::Damage;

		currentTime = 0;
		// �ǰ� �ִϸ��̼� ���
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), 0);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	// �׷��� �ʴٸ�
	else
	{

		//GameMode->SubtractCurrentEnemyLeft();
		//GameMode->CheckEnemyLeft();
		////GameMode->main_UI->PrintPlayerMoney();
		//GameMode->main_UI->PrintPlayerMoneyAnimation(target->money, target->money + 500);
		//target->money += 500;
		//GameMode->main_UI->PrintEarnMoney(500);

		////���� �ִϸ��̼� ���
		//anim->PlayDamageAnim(TEXT("die"));
		// ���¸� �������� ��ȯ
		mState = EEnemystate::Die;

		//isdying = true;

	}
	//�ִϸ��̼� ���� ����ȭ
	anim->animState = mState;
}

