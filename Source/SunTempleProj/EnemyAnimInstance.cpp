// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (IsValid(Pawn)) {
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimProperties()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (IsValid(Pawn)) {

			

			Enemy = Cast<AEnemy>(Pawn);
		}
	}
	if (IsValid(Pawn)) {
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MoveSpeed = LateralSpeed.Size();
	}
}
