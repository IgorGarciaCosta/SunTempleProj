// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainChar.h"


void UMainAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (IsValid(Pawn)) {
			MainChar = Cast<AMainChar>(Pawn);
		}
	}


}

void UMainAnimInstance::UpdateAnimProperties()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MoveSpeed = LateralSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		if (!IsValid(MainChar)) {
			MainChar = Cast<AMainChar>(Pawn);
		}
	}
}
