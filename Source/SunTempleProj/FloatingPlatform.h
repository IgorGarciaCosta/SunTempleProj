// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class SUNTEMPLEPROJ_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		FVector StartPoint;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
		FVector EndPoint;

	UPROPERTY(EditAnywhere)
		float interpSpeed = 2.f;
	
	UPROPERTY(EditAnywhere)
		float interpTime = 1.f;

	FTimerHandle InterpTimer;

	float distance;

	UPROPERTY(EditAnywhere)
	bool bInterping = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterping();

	void SwapVectors(FVector& vec1, FVector& vec2);

};
