// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundProgressBarWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class FORESTSTAYALIVE_API URoundProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

	
protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UImage> Icon;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UImage> ProgressBar;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Material")
	TObjectPtr<UMaterialInstanceDynamic> RoundProgressBar;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMaterialInstance> ProgressBarMI;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "UI")
	float InitPercent = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "UI")
	TObjectPtr<UTexture2D> InitIconImage;
	
	

	UFUNCTION(BlueprintCallable)
	void SetPercent(const float NewPercent);
	UFUNCTION(BlueprintCallable)
	void SetIconImage(UTexture2D* NewIconImage);
};
