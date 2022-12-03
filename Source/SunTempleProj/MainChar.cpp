// Fill out your copyright notice in the Description page of Project Settings.


#include "MainChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMainChar::AMainChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void AMainChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this,  &AMainChar::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainChar::StopJumping);


}

void AMainChar::MoveForward(float value)
{
	if ((Controller != nullptr) && (value != 0.0f)) {
		//find out wich way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void AMainChar::MoveRight(float value)
{
	if ((Controller != nullptr) && (value != 0.0f)) {
		//find out wich way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void AMainChar::Jump()
{
}

void AMainChar::StopJumping()
{
}

void AMainChar::TurnAtRate(float rate)
{
	AddControllerYawInput(rate * baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainChar::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * baseLookUpRate * GetWorld()->GetDeltaSeconds());

}

