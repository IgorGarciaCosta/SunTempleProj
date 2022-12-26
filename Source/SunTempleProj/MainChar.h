// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainChar.generated.h"

UCLASS()
class SUNTEMPLEPROJ_API AMainChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainChar();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float baseTurnRate = 65.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float baseLookUpRate = 65.f;




	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Player stats")
	float health = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
	float maxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
	float stamina = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
	float maxStamina = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
	int32 coins = 0;

	void DecrementHealth(float amount);

	void IncrementCoins(int32 amount);

	void Die();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void MoveForward(float value);
	void MoveRight(float value);

	void TurnAtRate(float rate);
	void LookUpAtRate(float rate);

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const{ return FollowCamera; }
};
