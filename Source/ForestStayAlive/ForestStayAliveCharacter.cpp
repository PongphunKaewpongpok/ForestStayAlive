// Fill out your copyright notice in the Description page of Project Settings.


#include "ForestStayAliveCharacter.h"

#include "AttributeComponent.h"
#include "ForestStayAlivePlayerController.h"
#include "Camera/CameraComponent.h"

void AForestStayAliveCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AForestStayAlivePlayerController* PC = Cast<AForestStayAlivePlayerController>(NewController);
	if (!IsValid(PC)) return;
	
	PC->ForestStayAliveCharacter = this;
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

void AForestStayAliveCharacter::SetTargetFOV(const float FOV)
{
	TargetFOV = FOV;
}

void AForestStayAliveCharacter::SetCameraFOV(const float FOV) const
{
	if (!MyFirstPersonCamera) return;
	
	MyFirstPersonCamera->FieldOfView = FOV;
}

