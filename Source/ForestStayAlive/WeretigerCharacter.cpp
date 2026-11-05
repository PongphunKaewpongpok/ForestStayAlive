// Fill out your copyright notice in the Description page of Project Settings.


#include "WeretigerCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "ForestStayAliveCharacter.h"
#include "ForestStayAlive/AI/WeretigerAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AWeretigerCharacter::AWeretigerCharacter()
{
	TigerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TigerMesh"));
	TigerMesh->SetupAttachment(GetCapsuleComponent());
	TigerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -95.0f));
	TigerMesh->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	TigerMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AWeretigerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWeretigerCharacter, Form);
}

FString AWeretigerCharacter::GetForm()
{
	return Form;
}

void AWeretigerCharacter::SwitchForm()
{
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	
	if (Form.Equals("Human"))
	{
		Multicast_StartTranformToTiger();
		Form = TEXT("Tiger");
	}
	else
	{
		Form = TEXT("Human");
	}
}

void AWeretigerCharacter::Execute(AForestStayAliveCharacter* Target)
{
	if (!HasAuthority() || !IsValid(Target)) return;
	
	Multicast_Execute();
	Target->GotExecuted(this);
	
	AAIController* AICon = Cast<AAIController>(GetController());
	if (!AICon) return;
	
	UBlackboardComponent* BB = AICon->GetBlackboardComponent();
	if (BB == nullptr) return;
	
	
	if (IsValid(AICon->BrainComponent))
	{
		AICon->BrainComponent->PauseLogic(TEXT("Execution Paused"));
	}
	
	
	
	FVector Start = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		Start,
		TargetLocation
	);

	SetActorRotation(FRotator(
		0.f,
		LookAtRotation.Yaw,
		0.f
	));
	
	
	UAnimInstance* AnimInstance = TigerMesh->GetAnimInstance();
	if (!AnimInstance) return;
	
	if (ExecuteAnimation)
	{
		AnimInstance->Montage_Play(ExecuteAnimation);
		
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AWeretigerCharacter::OnExecuteEnded);

		AnimInstance->Montage_SetEndDelegate(EndDelegate, ExecuteAnimation);
	}
}

void AWeretigerCharacter::OnExecuteEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!HasAuthority()) return;
	
	AAIController* AICon = Cast<AAIController>(GetController());
	if (!AICon) return;

	AICon->BrainComponent->ResumeLogic(TEXT("Execution Finished"));
	
	AWeretigerAIController* AIConWeretiger = Cast<AWeretigerAIController>(AICon);
	AIConWeretiger->FormChangedEvent();
}

void AWeretigerCharacter::OnStartTranformToTigerEnded(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->SetVisibility(false);
	TigerMesh->SetVisibility(true);
}

void AWeretigerCharacter::StopOnGameEnded() const
{
	AAIController* AICon = Cast<AAIController>(GetController());
	if (!AICon) return;
	
	if (IsValid(AICon->BrainComponent))
	{
		AICon->BrainComponent->PauseLogic(TEXT("Game End Paused"));
	}
}

void AWeretigerCharacter::Multicast_StartTranformToTiger_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	
	if (StartTranformToTigerAnimation)
	{
		AnimInstance->Montage_Play(StartTranformToTigerAnimation);
		
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AWeretigerCharacter::OnStartTranformToTigerEnded);

		AnimInstance->Montage_SetEndDelegate(EndDelegate, StartTranformToTigerAnimation);
	}
}

void AWeretigerCharacter::Multicast_Execute_Implementation()
{
	UAnimInstance* AnimInstance = TigerMesh->GetAnimInstance();
	if (!AnimInstance) return;
	
	if (ExecuteAnimation)
	{
		AnimInstance->Montage_Play(ExecuteAnimation);
	}
}
