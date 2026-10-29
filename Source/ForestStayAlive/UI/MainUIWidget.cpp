// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIWidget.h"

#include "Components/PlayerCrosshairWidget.h"
#include "Components/ProgressBar.h"
#include "Components/RoundProgressBarWidget.h"

void UMainUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainUIWidget::UpdateSanityBar(const float NewPercent) const
{
	SanityBar->SetPercent(NewPercent);
}

void UMainUIWidget::UpdateHungerBar(const float NewPercent) const
{
	HungerBar->SetPercent(NewPercent);
}

void UMainUIWidget::UpdateThirstBar(const float NewPercent) const
{
	ThirstBar->SetPercent(NewPercent);
}

void UMainUIWidget::UpdateHealthBar(const float NewPercent) const
{
	HealthBar->SetPercent(NewPercent);
}
