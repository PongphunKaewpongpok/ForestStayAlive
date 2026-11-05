// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimalCharacter.h"

#include "DiscoveryBookManager.h"
#include "DataAssets/DiscoveryBookData.h"

// Sets default values
ABaseAnimalCharacter::ABaseAnimalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseAnimalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseAnimalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UDiscoveryBookData* ABaseAnimalCharacter::GetDiscoveryBookData_Implementation() const
{
	return DiscoveryData;
}

bool ABaseAnimalCharacter::AddNewDataToDiscoveryBook_Implementation() const
{
	if (!IsValid(DiscoveryData)) return false;

	UGameInstance* GameInstance = GetGameInstance();
	if (!IsValid(GameInstance)) return false;

	UDiscoveryBookManager* DiscoveryManager = GameInstance->GetSubsystem<UDiscoveryBookManager>();
	if (!IsValid(DiscoveryManager)) return false;

	DiscoveryManager->UnlockDiscoveryName(DiscoveryData->DiscoveryID);

	return true;
}