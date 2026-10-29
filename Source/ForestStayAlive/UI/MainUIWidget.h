// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIWidget.generated.h"

class AForestStayAlivePlayerController;
class UPlayerCrosshairWidget;
class AForestStayAliveCharacter;
class UProgressBar;
class URoundProgressBarWidget;
/**
 * 
 */
UCLASS()
class FORESTSTAYALIVE_API UMainUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<URoundProgressBarWidget> SanityBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<URoundProgressBarWidget> HungerBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<URoundProgressBarWidget> ThirstBar;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UProgressBar> HealthBar;
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateSanityBar(const float NewPercent) const;
	UFUNCTION(BlueprintCallable)
	void UpdateHungerBar(const float NewPercent) const;
	UFUNCTION(BlueprintCallable)
	void UpdateThirstBar(const float NewPercent) const;
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(const float NewPercent) const;
	
	UPROPERTY()
	AForestStayAlivePlayerController* PlayerController;
};
