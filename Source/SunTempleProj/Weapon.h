// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponState :uint8 {
	EWS_Pickup		UMETA(DiaplayName = "Pickup"),
	EWS_Equipped	UMETA(DiaplayName = "Equipped"),

	EWS_MAX UMETA(DiaplayName = "DefaultMax"),
};

/**
 * 
 */
UCLASS()
class SUNTEMPLEPROJ_API AWeapon : public AItem
{
	GENERATED_BODY()

	

public:

	AWeapon();

	UPROPERTY(EditDefaultsOnly)
		FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponState WeaponState = EWeaponState::EWS_Pickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Particles")
	bool bWeaponParticles = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	class USoundCue* OnEquippedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
		class USoundCue* SwingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
		class UBoxComponent* CombatCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
		float Damage = 25.f;




protected:
	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void Equip(class AMainChar* Char);

	FORCEINLINE void SetWeaponState(EWeaponState state) { WeaponState = state; }
	FORCEINLINE EWeaponState GetWeaponState(){return WeaponState;}

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
		void EndCollision();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AController* WeaponInstigator;

	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }
};
