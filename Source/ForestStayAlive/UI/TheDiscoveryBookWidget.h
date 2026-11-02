// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ForestStayAlive/DiscoveryBookEntryProgress.h"
#include "TheDiscoveryBookWidget.generated.h"

class UDiscoveryBookManager;
class UButton;
class UVerticalBox;
class UTextBlock;
class UImage;
class UDiscoveryBookData;
/**
 * 
 */
UCLASS()
class FORESTSTAYALIVE_API UTheDiscoveryBookWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	UDiscoveryBookManager* DiscoveryBookManager;
	
	
	FName Category = FName("Plants");
	int32 PageIndex = 0;
	
	void ClearDetailBoxes();
	void AddDetailText(UVerticalBox* TargetBox, const FText& Text);
	void UpdateDetailSection(UVerticalBox* TargetBox, const TMap<FName, FText>& Details, EDiscoveryBookSection Section, FName DiscoveryID);

protected:
	virtual void NativeConstruct() override;
	
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> DiscoveryName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UImage> CaptureImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UVerticalBox> AppearancesBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UVerticalBox> BenefitsBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UVerticalBox> PrecautionsBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UVerticalBox> FoundInLocationsBox;
	
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UButton> PlantsButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UButton> AnimalsButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UButton> PreviousPage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UButton> NextPage;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UTextBlock> TemplateDetail;
	
	
	
	UFUNCTION()
	void OnPlantsButtonClicked();
	UFUNCTION()
	void OnAnimalsButtonClicked();
	
	UFUNCTION()
	void OnPreviousPageClicked();
	UFUNCTION()
	void OnNextPageClicked();
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdatePage();
};
