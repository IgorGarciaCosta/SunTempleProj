// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainChar.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());

	CollisionVolume->SetSphereRadius(100.f);
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (WeaponState == EWeaponState::EWS_Pickup && OtherActor) {
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);
		if (IsValid(mainChar)) {
			mainChar->SetActiveOverlappingItem(this);
			
		}
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);
		if (IsValid(mainChar)) {
			mainChar->SetActiveOverlappingItem(nullptr);

		}
	}
}

void AWeapon::Equip(AMainChar* Char)
{
	if (IsValid(Char)) {
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");
		if (IsValid(RightHandSocket)) {
			RightHandSocket->AttachActor(this, Char->GetMesh());
			bRotate = false;
			
			Char->SetEquippedWeapon(this);
			Char->SetActiveOverlappingItem(nullptr);
		}
		if (IsValid(OnEquippedSound)) {
			UGameplayStatics::PlaySound2D(this, OnEquippedSound);
		}
		if (!bWeaponParticles) {
			IdleParticlesComponent->Deactivate();
		}

	}
}
