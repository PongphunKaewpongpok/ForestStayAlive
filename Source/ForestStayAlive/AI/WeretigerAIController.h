// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ForestStayAlive/AI/BaseAIController.h"
#include "WeretigerAIController.generated.h"

/**
 * 
 */
UCLASS()
class FORESTSTAYALIVE_API AWeretigerAIController : public ABaseAIController
{
	GENERATED_BODY()
	
	FTimerHandle FormChangedTimerHandle;
	
protected:
	virtual void OnTargetSensed(AActor* Actor, FAIStimulus Stimulus) override;
	
public:
	void FormChangedEvent(); 
};
