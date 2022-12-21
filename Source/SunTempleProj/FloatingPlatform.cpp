// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;

	distance = (EndPoint - StartPoint).Size();
	
	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, interpTime);
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	if (bInterping) {
		Super::Tick(DeltaTime);
		FVector currentLocation = GetActorLocation();
		FVector interp = FMath::VInterpTo(currentLocation, EndPoint, DeltaTime, interpSpeed);
		SetActorLocation(interp);

		float distanceTraveled = (GetActorLocation() - StartPoint).Size();
		if (distance - distanceTraveled <=1.f) {
			ToggleInterping();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, interpTime);
			SwapVectors(StartPoint, EndPoint);
		}
	}
	
	
}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;

}

void AFloatingPlatform::SwapVectors(FVector& vec1, FVector& vec2)
{
	FVector temp = vec1;
	vec1 = vec2;
	vec2 = temp;
}

