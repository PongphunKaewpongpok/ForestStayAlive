// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCrosshairWidget.generated.h"

class URoundProgressBarWidget;
/**
 * 
 */
UCLASS()
class FORESTSTAYALIVE_API UPlayerCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

	const float InteractDuration = 1.0f;
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<URoundProgressBarWidget> Crosshair;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTexture2D> Crosshair_Normal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTexture2D> Crosshair_Interactable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTexture2D> Crosshair_Interacting;
	
public:
	bool bIsInteracting = false;
	float InteractProgress = 0.0f;
	
	UFUNCTION(BlueprintCallable)
	void SetCrosshairType(const FText& NewType);
};
