// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

#include "ForestStayAlivePlayerController.h"
#include "ForestStayAliveCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/MainUIWidget.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const AForestStayAliveCharacter* PlayerCharacter = Cast<AForestStayAliveCharacter>(GetOwner()))
	{
		OwnPlayerController = Cast<AForestStayAlivePlayerController>(PlayerCharacter->GetController());
	}
	
	StopSprint();
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSprinting)
	{
		ModifyStamina(-5 * DeltaTime);
		ModifyHunger(-0.1 * DeltaTime);
		ModifyThirst(-0.1 * DeltaTime);
	}
	else
	{
		ModifyStamina(5 * DeltaTime);
		ModifyHunger(-0.01 * DeltaTime);
		ModifyThirst(-0.01 * DeltaTime);
	}
	ModifySanity(-0.01 * DeltaTime);
}



// ----------------------- Health -----------------------
void UAttributeComponent::ApplyDamage(const float Amount)
{
	if (Amount < 0.0f) { return; }
	Health = FMath::Max(
		Health - Amount,
		0.0f
	);
	
	if (!OwnPlayerController || !OwnPlayerController->MainUIWidget) return;
	OwnPlayerController->MainUIWidget->UpdateHealthBar(GetHealthPercent());
}

void UAttributeComponent::Heal(const float Amount)
{
	if (Amount < 0.0f) { return; }
	Health = FMath::Min(
		Health + Amount,
		MaxHealth
	);
	
	if (!OwnPlayerController || !OwnPlayerController->MainUIWidget) return;
	OwnPlayerController->MainUIWidget->UpdateHealthBar(GetHealthPercent());
}

float UAttributeComponent::GetHealthPercent() const
{
	if (Health <= 0.0f) { return 0.0f; }
	return Health / MaxHealth;
}

bool UAttributeComponent::IsDead() const {
	return Health <= 0.0f;
}
// ------------------------------------------------------



// ----------------------- Stamina -----------------------
void UAttributeComponent::ModifyStamina(const float Amount)
{
	Stamina = FMath::Clamp(
		Stamina + Amount,
		0.0f,
		MaxStamina
	);

	if (Stamina <= 0.0f)
	{
		StopSprint();
	}
}

float UAttributeComponent::GetStaminaPercent() const
{
	if (Stamina <= 0.0f) { return 0.0f; }
	return Stamina / MaxStamina;
}
// -------------------------------------------------------



// ----------------------- Sprint -----------------------
bool UAttributeComponent::IsSprinting()
{
	return bIsSprinting;
}

void UAttributeComponent::StartSprint()
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) { return; }

	UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
	if (!CharacterMovement) { return; }
	
	CharacterMovement->MaxWalkSpeed = GetRunSpeed();
	bIsSprinting = true;
}

void UAttributeComponent::StopSprint()
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) { return; }

	UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
	if (!CharacterMovement) { return; }
	
	CharacterMovement->MaxWalkSpeed = GetWalkSpeed();
	bIsSprinting = false;
}

float UAttributeComponent::GetWalkSpeed() const
{
	return WalkSpeed;
}

float UAttributeComponent::GetRunSpeed() const
{
	return RunSpeed;
}
// ------------------------------------------------------



// ----------------------- Hunger -----------------------
void UAttributeComponent::ModifyHunger(const float Amount)
{
	Hunger = FMath::Clamp(
		Hunger + Amount,
		0.0f,
		MaxHunger
	);
	
	if (Hunger <= 0.0f)
	{
		ApplyDamage(1);
	}
	
	if (!OwnPlayerController || !OwnPlayerController->MainUIWidget) return;
	OwnPlayerController->MainUIWidget->UpdateHungerBar(GetHungerPercent());
}

float UAttributeComponent::GetHungerPercent() const
{
	if (Hunger <= 0.0f) { return 0.0f; }
	return Hunger / MaxHunger;
}
// ------------------------------------------------------



// ----------------------- Thirst -----------------------
void UAttributeComponent::ModifyThirst(const float Amount)
{
	Thirst = FMath::Clamp(
		Thirst + Amount,
		0.0f,
		MaxThirst
	);
	
	if (Thirst <= 0.0f)
	{
		ApplyDamage(1);
	}
	
	if (!OwnPlayerController || !OwnPlayerController->MainUIWidget) return;
	OwnPlayerController->MainUIWidget->UpdateThirstBar(GetThirstPercent());
}

float UAttributeComponent::GetThirstPercent() const
{
	if (Thirst <= 0.0f) { return 0.0f; }
	return Thirst / MaxThirst;
}
// ------------------------------------------------------



// ----------------------- Sanity -----------------------
float UAttributeComponent::GetSanityPercent() const
{
	if (Sanity <= 0.0f) { return 0.0f; }
	return Sanity / MaxSanity;
}

void UAttributeComponent::ModifySanity(const float Amount)
{
	Sanity = FMath::Clamp(
		Sanity + Amount,
		0.0f,
		MaxSanity
		);
	
	if (!OwnPlayerController || !OwnPlayerController->MainUIWidget) return;
	OwnPlayerController->MainUIWidget->UpdateSanityBar(GetSanityPercent());
}
// ------------------------------------------------------