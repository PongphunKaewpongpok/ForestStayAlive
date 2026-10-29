// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundProgressBarWidget.h"

#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"


void URoundProgressBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetPercent(InitPercent);
	SetIconImage(InitIconImage);
}

void URoundProgressBarWidget::SetPercent(const float NewPercent)
{
	if (!IsValid(RoundProgressBar))
	{
		if (!IsValid(ProgressBarMI) || !IsValid(ProgressBar)) return;

		RoundProgressBar = UMaterialInstanceDynamic::Create(ProgressBarMI, this);
		if (!IsValid(RoundProgressBar)) return;

		ProgressBar->SetBrushFromMaterial(RoundProgressBar);
	}

	RoundProgressBar->SetScalarParameterValue(
		TEXT("Percent"),
		FMath::Clamp(NewPercent, 0.0f, 1.0f)
	);
}

void URoundProgressBarWidget::SetIconImage(UTexture2D* NewIconImage)
{
	if (!Icon || !NewIconImage) return;
	
	Icon->SetBrushFromTexture(NewIconImage);
}
