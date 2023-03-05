// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MainChar.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AgroSphere = CreateDefaultSubobject<USphereComponent>("AgroSPhere");
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->SetSphereRadius(600.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>("CombatSPhere");
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->SetSphereRadius(75.f);

	CombatCollision = CreateDefaultSubobject<UBoxComponent>("CombatCollision");
	CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));
	
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAgroOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAgroOverlapEnd);
	
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCombatSphereOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnCombatSphereOverlapEnd);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

}

void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor)) {
		AMainChar* touchedPlayer = Cast<AMainChar>(OtherActor);
		if (IsValid(touchedPlayer)) {
			
			if (IsValid(touchedPlayer->HitParticle)) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), touchedPlayer->HitParticle, touchedPlayer->GetTransform(), false);
			}

			if (touchedPlayer->HitSound) {
				UGameplayStatics::PlaySound2D(GetWorld(), touchedPlayer->HitSound);
			}
			if (DamageTypeClass) {
				UGameplayStatics::ApplyDamage(touchedPlayer, Damage, AIController, this, DamageTypeClass);
			}

		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnAgroOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && Alive()) {
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);
		if (IsValid(mainChar)) {
			MoveToTarget(mainChar);
		}
	}
}

void AEnemy::OnAgroOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor)) {
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);
		if (mainChar) {
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (AIController) {
				AIController->StopMovement();
			}
		}
	}
}

void AEnemy::OnCombatSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && Alive()) {
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);
		if (mainChar) {
			mainChar->SetCombatTarget(this);
			CombatTarget = mainChar;
			bOverlappingCombatSphere = true;
			Attack();
			//SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
	}
}

void AEnemy::OnCombatSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor)) {
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);
		if (mainChar) {
			if (mainChar->CombatTarget == this) {
				mainChar->SetCombatTarget(nullptr);
			}

			
			bOverlappingCombatSphere = false;
			/*SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
			MoveToTarget(mainChar);*/
			if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking) {
				MoveToTarget(mainChar);
				CombatTarget = nullptr;
			}
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}

void AEnemy::MoveToTarget(AMainChar* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (IsValid(AIController)) {
		UE_LOG(LogTemp, Warning, TEXT("Moving To Target"));
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(5.f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		/*TArray<FNavPathPoint> PathPoints =NavPath->GetPathPoints();
		for (auto Point : PathPoints) {
			FVector loca = Point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, loca, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);

		}*/
	}
}

void AEnemy::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (IsValid(SwingSound)) {
		UGameplayStatics::PlaySound2D(GetWorld(), SwingSound);
	}
}

void AEnemy::EndCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	if (!Alive()) return;
	if (IsValid(AIController)) {
		AIController->StopMovement();
		//(LogTemp, Warning, TEXT("here"));
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
	}

	if (!bAttacking) {
		bAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance)) {
			AnimInstance->Montage_Play(CombatMontage, 1.35f);
			AnimInstance->Montage_JumpToSection("Attack", CombatMontage);
		}
		
	}
}

void AEnemy::AttackEnd()
{
	bAttacking = false;

	if (bOverlappingCombatSphere) {
		float AttackTime = FMath::FRandRange(AttckMinTime, AttckMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
		//Attack();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f) {
		Health -= DamageAmount;
		Die();
	}
	else {
		Health -= DamageAmount;
	}
	return DamageAmount;
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance)) {
		AnimInstance->Montage_Play(CombatMontage, 1.35f);
		AnimInstance->Montage_JumpToSection("Death", CombatMontage);
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DeathTimer,this, &AEnemy::Disappear,DeathDelay);
}

bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Disappear()
{
	Destroy();
}

