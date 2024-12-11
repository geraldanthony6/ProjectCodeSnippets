// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Enemy.h"

void AEnemyController::MakeAttackDecision(APawn* targetPawn)
{
	auto controlledCharacter = Cast<AEnemy>(GetPawn());
	auto dist = FVector::Dist2D(targetPawn->GetActorLocation(), GetPawn()->GetTargetLocation());

	if(dist <= controlledCharacter->AttackRange && controlledCharacter->CanAttack())
	{
		controlledCharacter->Attack();
	}
	else
	{
		controlledCharacter->Chase(targetPawn);
	}
}
