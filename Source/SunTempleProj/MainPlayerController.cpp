// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/USerWIdget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HUDOverlayAsset)) {
		HUDOverlay = CreateWidget(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();

	HUDOverlay->SetVisibility(ESlateVisibility::Visible);
}
