// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ForestStayAlivePlayerController.generated.h"

class UPlayerCrosshairWidget;
class AForestStayAliveCharacter;
struct FInputActionValue;
class UInputAction;
class UCameraWidget;
class UMainUIWidget;
/**
 * 
 */
UCLASS()
class FORESTSTAYALIVE_API AForestStayAlivePlayerController : public APlayerController
{
	GENERATED_BODY()

	
protected:
	virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
	
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* RightClickAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LeftClickAction;

	void OnRightClickStarted(const FInputActionValue& Value);
	
	void OnLeftClickHold(const FInputActionValue& Value);
	void OnLeftClickCompleted(const FInputActionValue& Value);
public:
	virtual void Tick(float DeltaSeconds) override;
	
	
	UPROPERTY(BlueprintReadOnly)
	UMainUIWidget* MainUIWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMainUIWidget> MainUIWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	UCameraWidget* CameraWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCameraWidget> CameraWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UPlayerCrosshairWidget* PlayerCrosshairWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlayerCrosshairWidget> PlayerCrosshairWidgetClass;
	
	UPROPERTY()
	AForestStayAliveCharacter* ForestStayAliveCharacter;
	
	
};
