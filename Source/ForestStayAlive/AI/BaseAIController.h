// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "BaseAIController.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;
/**
 * 
 */
UCLASS()
class FORESTSTAYALIVE_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnTargetSensed(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(EditAnywhere, Category="AI")
	UBehaviorTree* BehaviorTreeAsset;
	
	// Sense Config
	UPROPERTY(VisibleAnywhere, Category="AI")
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleAnywhere, Category="AI")
	UAISenseConfig_Hearing* HearingConfig;
	UPROPERTY(VisibleAnywhere, Category="AI")
	UAISenseConfig_Damage* DamageConfig;
	
	
	// Blackboard Key
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName TargetActorKey = TEXT("TargetActor");
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName DamageLocationKey = TEXT("LastDamageLocation");
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName InvestigateLocationKey = TEXT("InvestigateLocation");
};
