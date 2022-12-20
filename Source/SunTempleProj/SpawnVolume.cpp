// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Critter.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>("SpawningBox");

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();

	FVector point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	
	return point;
}

void  ASpawnVolume::SpawnPawn_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (IsValid(ToSpawn)) {
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		if (IsValid(World)) {
			ACritter* SpawnedCritter =  World->SpawnActor<ACritter>(ToSpawn, Location, FRotator(0.f), SpawnParams);
		}
	}
}

