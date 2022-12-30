// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "MainChar.h"

AExplosive::AExplosive()
{
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor,  OtherComp,  OtherBodyIndex,  bFromSweep, SweepResult);
	if (IsValid(OtherActor)) {
		AMainChar* charRef = Cast<AMainChar>(OtherActor);
		if (IsValid(charRef)) {
			charRef->DecrementHealth(Damage);
			Destroy();
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd( OverlappedComponent,  OtherActor,  OtherComp,  OtherBodyIndex);
}
