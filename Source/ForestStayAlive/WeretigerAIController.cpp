// Fill out your copyright notice in the Description page of Project Settings.


#include "WeretigerAIController.h"

void AWeretigerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}