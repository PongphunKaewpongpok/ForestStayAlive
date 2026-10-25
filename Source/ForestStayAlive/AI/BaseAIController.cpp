// Fill out your copyright notice in the Description page of Project Settings.



#include "BaseAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"

ABaseAIController::ABaseAIController()
{
	// This component must exist before any sense can be handed to it.
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	SightConfig->SightRadius = 1200.f;
	SightConfig->LoseSightRadius = 1500.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AAIController::GetPerceptionComponent()->ConfigureSense(*SightConfig);
	
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));
	HearingConfig->HearingRange = 3000.f;
	HearingConfig->SetMaxAge(8.f);
	
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	AAIController::GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage"));
	DamageConfig->SetMaxAge(8.f);
	AAIController::GetPerceptionComponent()->ConfigureSense(*DamageConfig);

	AAIController::GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UAIPerceptionComponent* Perception = GetPerceptionComponent())
	{
		Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetSensed);
	}
}

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseAIController::OnTargetSensed(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (BB == nullptr || Actor == nullptr) { return; }
	
	const TSubclassOf<UAISense> Sense = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);
	
	if (Sense == UAISense_Sight::StaticClass())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BB->SetValueAsObject(TargetActorKey, Actor);
		}
		else
		{	
			const AActor* LastTargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey));
			BB->SetValueAsVector(InvestigateLocationKey, LastTargetActor->GetActorLocation());
			
			BB->ClearValue(TargetActorKey);
		}
		return;
	}
	
	if (Sense == UAISense_Damage::StaticClass())
	{
		BB->SetValueAsVector(DamageLocationKey, Stimulus.StimulusLocation);
		return;
	}
	
	if (Sense == UAISense_Hearing::StaticClass())
	{
		BB->SetValueAsVector(InvestigateLocationKey, Stimulus.StimulusLocation);
	}
}
