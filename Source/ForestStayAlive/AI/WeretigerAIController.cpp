// Fill out your copyright notice in the Description page of Project Settings.


#include "WeretigerAIController.h"

#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ForestStayAlive/WeretigerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ForestStayAlive/ForestStayAliveCharacter.h"

void AWeretigerAIController::OnTargetSensed(AActor* Actor, FAIStimulus Stimulus)
{
	const TSubclassOf<UAISense> Sense = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);
	
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (BB == nullptr || Actor == nullptr) return;
	
	AWeretigerCharacter* Me = Cast<AWeretigerCharacter>(GetCharacter());
	if (Me == nullptr) return;
	
	AForestStayAliveCharacter* PlayerCharacter = Cast<AForestStayAliveCharacter>(Actor);
	if (!IsValid(PlayerCharacter)) return;
	
	
	
	if (Sense == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BB->SetValueAsObject(TargetActorKey, Actor);
		}
		else
		{
			const AActor* LastTargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey));
			
			if (IsValid(LastTargetActor))
			{
				BB->SetValueAsVector(
					InvestigateLocationKey,
					LastTargetActor->GetActorLocation()
				);
			}
			
			FormChangedEvent();
		}
			
		return;
	}
	
	if (Sense == UAISense_Hearing::StaticClass())
	{
		BB->SetValueAsVector(InvestigateLocationKey, Stimulus.StimulusLocation);
		return;
	}
}

void AWeretigerAIController::FormChangedEvent()
{
	if (!HasAuthority()) return;
	
	AWeretigerCharacter* Me = Cast<AWeretigerCharacter>(GetCharacter());
	if (Me == nullptr) return;
	
	UCharacterMovementComponent* Movement = Me->GetCharacterMovement();
	if (!IsValid(Movement)) return;
	
	if (Me->GetForm().Equals("Human"))
	{
		Me->GetCharacterMovement()->MaxWalkSpeed = Me->HumanFormSpeed;
	}
	else
	{
		Me->GetCharacterMovement()->MaxWalkSpeed = Me->TigerFormWanderSpeed;
	}
}
