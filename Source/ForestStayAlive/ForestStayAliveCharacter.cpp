// Fill out your copyright notice in the Description page of Project Settings.


#include "ForestStayAliveCharacter.h"

#include "ForestStayAlive/Components/AttributeComponent.h"
#include "ForestStayAlivePlayerController.h"
#include "WeretigerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void AForestStayAliveCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AForestStayAlivePlayerController* PC = Cast<AForestStayAlivePlayerController>(NewController);
	if (!IsValid(PC)) return;
	
	PC->ForestStayAliveCharacter = this;
}

void AForestStayAliveCharacter::GotExecuted(AWeretigerCharacter* Weretiger)
{
	if (!Weretiger) return;
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!IsValid(PC)) return;
	
	
	GetCharacterMovement()->StopMovementImmediately();
	SetCanJump(false);
	PC->SetIgnoreMoveInput(true);
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	
	FVector Start = GetActorLocation();
	FVector TargetLocation = Weretiger->GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		Start,
		TargetLocation
	);

	SetActorRotation(FRotator(
		0.f,
		LookAtRotation.Yaw,
		0.f
	));
	
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;
	
	if (DeathAnimation)
	{
		AnimInstance->Montage_Play(DeathAnimation);
	}
}

// Sets default values
AForestStayAliveCharacter::AForestStayAliveCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
}

// Called when the game starts or when spawned
void AForestStayAliveCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AForestStayAliveCharacter* ForestCharacter = Cast<AForestStayAliveCharacter>(GetOwner());
	if (IsValid(ForestCharacter))
	{
		MyFirstPersonCamera = ForestCharacter->FindComponentByClass<UCameraComponent>();
	}
}

// Called every frame
void AForestStayAliveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AForestStayAliveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AForestStayAliveCharacter::SetCanJump(bool CanJump)
{
	bCanJump = CanJump;
}

void AForestStayAliveCharacter::Jump()
{
	if (!bCanJump) { return; }
	
	Super::Jump();
}



void AForestStayAliveCharacter::SetTargetFOV(const float FOV)
{
	TargetFOV = FOV;
}

void AForestStayAliveCharacter::SetCameraFOV(const float FOV) const
{
	if (!MyFirstPersonCamera) return;
	
	MyFirstPersonCamera->FieldOfView = FOV;
}

