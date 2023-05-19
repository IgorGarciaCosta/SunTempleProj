// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FirstSaveGame.generated.h"


USTRUCT(BlueprintType)
struct FCharacterStats {
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere)
		float Health = 0;
	UPROPERTY(VisibleAnywhere)
		float MaxHealth = 0;
	UPROPERTY(VisibleAnywhere)
		float Stamina = 0;
	UPROPERTY(VisibleAnywhere)
		float MaxStamina = 0;
	UPROPERTY(VisibleAnywhere)
		int32 Coins = 0;
	UPROPERTY(VisibleAnywhere)
		FVector Loc = FVector();
	UPROPERTY(VisibleAnywhere)
		FRotator Rot = FRotator();
	UPROPERTY(VisibleAnywhere)
		FString WeaponName = TEXT("");

	UPROPERTY(VisibleAnywhere)
		FString LvlName = TEXT("");
};
/**
 * 
 */
UCLASS()
class SUNTEMPLEPROJ_API UFirstSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UFirstSaveGame();

	UPROPERTY(VisibleAnywhere)
		FString PlayerName = "Default";

	UPROPERTY(VisibleAnywhere)
		uint32 UserIndex= 0;
	
	UPROPERTY(VisibleAnywhere)
		FCharacterStats CharStats;
};
