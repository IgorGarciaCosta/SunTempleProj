// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "MainChar.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"

AExplosive::AExplosive()
{
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor,  OtherComp,  OtherBodyIndex,  bFromSweep, SweepResult);
	if (IsValid(OtherActor)) {
		AMainChar* charRef = Cast<AMainChar>(OtherActor);
		if (IsValid(charRef)) {
			if (IsValid(OverlapParticles)) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}
			if (OverlapSound) {
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			charRef->DecrementHealth(Damage);
			Destroy();
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd( OverlappedComponent,  OtherActor,  OtherComp,  OtherBodyIndex);
}
