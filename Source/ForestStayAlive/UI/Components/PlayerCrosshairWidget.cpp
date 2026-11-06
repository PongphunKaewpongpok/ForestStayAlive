// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCrosshairWidget.h"

#include "RoundProgressBarWidget.h"

void UPlayerCrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bIsInteracting)
	{
		InteractProgress = FMath::Clamp(
			InteractProgress + (InDeltaTime / InteractDuration),
			0.0f,
			1.0f
		);
	}
	
	if (Crosshair)
	{
		Crosshair->SetPercent(InteractProgress);
	}
}

void UPlayerCrosshairWidget::SetCrosshairType(const ECrosshairType NewType)
{
	if (NewType == ECrosshairType::Normal)
	{
		Crosshair->SetIconImage(Crosshair_Normal);
	}
	else if (NewType == ECrosshairType::Interactable)
	{
		Crosshair->SetIconImage(Crosshair_Interactable);
	}
	else if (NewType == ECrosshairType::Interacting)
	{
		Crosshair->SetIconImage(Crosshair_Interacting);
	}
}
