// Fill out your copyright notice in the Description page of Project Settings.


#include "MainChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "Enemy.h"
#include "Weapon.h"

// Sets default values
AMainChar::AMainChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	GetCapsuleComponent()->SetCapsuleSize(46.f, 100.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//dont rotate when the contorller rotates. Affects onoy the camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);//how fast will rotate in each axis
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

}

void AMainChar::ShowPickUpLocations()
{

	for (FVector Location : PickupLocations) {
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 12, FLinearColor::Blue, 15.f, .5f);
	}

}

void AMainChar::SetMovementStatus(EMovementStatus status)
{
	MovementStatus = status;

	if (MovementStatus == EMovementStatus::EMS_Sprinting) {
		GetCharacterMovement()->MaxWalkSpeed = sprintingSpeed;
	}

	else {
		GetCharacterMovement()->MaxWalkSpeed = runningSpeed;
	}
}

void AMainChar::SetInterpToEnemy(bool Interp)
{
	bInterpEnemy = Interp;
}

FRotator AMainChar::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookATRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotation;
}

void AMainChar::ShiftKeyDown()
{
	bShiftKeyDown = true;
	//SetMovementStatus(EMovementStatus::EMS_Sprinting);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), bShiftKeyDown ? TEXT("true") : TEXT("false"));
}

void AMainChar::ShiftKeyUp()
{
	bShiftKeyDown = false;
	//SetMovementStatus(EMovementStatus::EMS_Normal);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), bShiftKeyDown ? TEXT("true") : TEXT("false"));
}

void AMainChar::DecrementHealth(float amount)
{

	if ((health - amount) >= 15.f) {
		health -= amount;
	}
	else Die();

}

float AMainChar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DecrementHealth(DamageAmount);

	return DamageAmount;
}

void AMainChar::IncrementCoins(int32 amount)
{
	coins += amount;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), coins);
}

void AMainChar::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage) {
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
}

// Called when the game starts or when spawned
void AMainChar::BeginPlay()
{
	Super::BeginPlay();

	//UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation()+FVector(0, 0, 75.f), 25.f, 12, FLinearColor::Blue, 5.f, 2.f);
	
}

// Called every frame
void AMainChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus) {
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown) {
			if (stamina - DeltaStamina <= MinSprintStamina) {
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				stamina -= DeltaStamina;
			}
			else {
				stamina -= DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}

		else {//shift key up
			if (stamina + DeltaStamina >= maxStamina) {
				stamina = maxStamina;
			}
			else {
				stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}

		break;

	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown) {
			if (stamina - DeltaStamina <= 0.f) {
				stamina = 0;
				SetStaminaStatus(EStaminaStatus::ESS_Exausted);
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else {
				stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}

		else {
			if (stamina + DeltaStamina >= MinSprintStamina) {
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				stamina += DeltaStamina;
			}
			else {
				stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}

		break;

	case EStaminaStatus::ESS_Exausted:
		if (bShiftKeyDown) {
			stamina = 0.f;
		}

		else {
			stamina += DeltaStamina;
			SetStaminaStatus(EStaminaStatus::ESS_ExaustedRecovering);
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
	break;
		
	case EStaminaStatus::ESS_ExaustedRecovering:
		if (stamina + DeltaStamina >= MinSprintStamina) {
			stamina += DeltaStamina;
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}

		else {
			stamina += DeltaStamina;
		}

		SetMovementStatus(EMovementStatus::EMS_Normal);

		break;

	default:
		break;
		

	}


	if (bInterpEnemy && CombatTarget) {
		FRotator lookYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), lookYaw, DeltaTime, InterpSpeed);
		
		SetActorRotation(InterpRotation);
	
	}
}

// Called to bind functionality to input
void AMainChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainChar::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	/*PlayerInputComponent->BindAxis("TurnRate", this, &AMainChar::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainChar::LookUpAtRate);*/


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this,  &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainChar::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainChar::ShiftKeyUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMainChar::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMainChar::LMBUp);


}

void AMainChar::MoveForward(float value)
{
	if ((Controller != nullptr) && (value != 0.0f) && !bAttacking) {
		//find out wich way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void AMainChar::MoveRight(float value)
{
	if ((Controller != nullptr) && (value != 0.0f) && !bAttacking) {
		//find out wich way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void AMainChar::TurnAtRate(float rate)
{
	AddControllerYawInput(rate * baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainChar::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * baseLookUpRate * GetWorld()->GetDeltaSeconds());

}

void AMainChar::LMBDown()
{
	
	bLMBDown = true;

	if (IsValid(ActiveOverlappingItem)) {
		AWeapon* weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (IsValid(weapon)) {
			weapon->Equip(this);
			//SetActiveOverlappingItem(nullptr);//to allow getting other weapons
		}
	}

	else if (IsValid(EquippedWeapon)) {
		Attack();
	}
}

void AMainChar::LMBUp()
{
	bLMBDown = false;
}

void AMainChar::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (IsValid(EquippedWeapon)) {
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}

void AMainChar::Attack()
{
	if (bAttacking == true) return;
	bAttacking = true;
	SetInterpToEnemy(true);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage) {
		int32 section = FMath::RandRange(0, 1);
		switch (section) {
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.2f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;

			case 1:
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;
			default:
				break;
		}
	}
	
}

void AMainChar::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);
	if (bLMBDown) {
		Attack();
	}
}

void AMainChar::PlaySwingSound()
{
	if (EquippedWeapon->SwingSound) {
		UGameplayStatics::PlaySound2D(GetWorld(), EquippedWeapon->SwingSound);

	}

}

