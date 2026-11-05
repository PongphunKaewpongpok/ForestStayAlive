// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Weretiger_HumanChase.h"

#include "AIController.h"
#include "ForestStayAlive/ForestStayAliveCharacter.h"
#include "ForestStayAlive/WeretigerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Weretiger_HumanChase::UBTTask_Weretiger_HumanChase()
{
	NodeName = TEXT("BTTask Weretiger HumanChase");
}

EBTNodeResult::Type UBTTask_Weretiger_HumanChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (AICon == nullptr) { return EBTNodeResult::Failed; }
	
	AWeretigerCharacter* Me = Cast<AWeretigerCharacter>(AICon->GetPawn());
	if (Me == nullptr) { return EBTNodeResult::Failed; }
	
	UBlackboardComponent* BB = AICon->GetBlackboardComponent();
	if (BB == nullptr) { return EBTNodeResult::Failed; }
	
	AForestStayAliveCharacter* Target = Cast<AForestStayAliveCharacter>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (Target == nullptr) { return EBTNodeResult::Failed; }
	
	
	
	constexpr float AttackRange = 200.f;

	const float DistanceSquared = FVector::DistSquared(
		Me->GetActorLocation(),
		Target->GetActorLocation()
	);
	
	
	
	if (Me->GetForm().Equals("Human"))
	{
		if (IsPlayerRunningAway(Me, Target))
		{
			Me->SwitchForm();
		}
		else
		{
			if (DistanceSquared <= FMath::Square(AttackRange))
			{
				Me->SwitchForm();
			}
			else
			{
				AICon->MoveToActor(Target);
			}
		}
	}
	else
	{
		if (IsValid(Me->TigerMesh) && Me->TigerMesh->IsVisible())
		{
			if (DistanceSquared <= FMath::Square(AttackRange))
			{
				Me->Execute(Target);
				BB->ClearValue(TargetActorKey.SelectedKeyName);
			}
			else
			{
				Me->GetCharacterMovement()->MaxWalkSpeed = Me->TigerFormChaseSpeed;
				AICon->MoveToActor(Target);
			}
			
		}
	}
	
	return EBTNodeResult::Succeeded;
}

bool UBTTask_Weretiger_HumanChase::IsPlayerRunningAway(ACharacter* Weretiger, ACharacter* Target) const
{
	if (!IsValid(Weretiger) || !IsValid(Target)) return false;

	FVector AwayDirection = Target->GetActorLocation() - Weretiger->GetActorLocation();

	AwayDirection.Z = 0.0f;
	AwayDirection.Normalize();

	FVector Velocity = Target->GetVelocity();
	Velocity.Z = 0.0f;

	if (Velocity.SizeSquared() < FMath::Square(100.0f)) return false;
	Velocity.Normalize();

	const float Dot = FVector::DotProduct(Velocity, AwayDirection);
	return Dot > 0.7f;
}