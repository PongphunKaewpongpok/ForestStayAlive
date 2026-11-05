// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ForestStayAlive/Interfaces/Photographable.h"
#include "GameFramework/Character.h"
#include "BaseAnimalCharacter.generated.h"

class UDiscoveryBookData;

UCLASS()
class FORESTSTAYALIVE_API ABaseAnimalCharacter : public ACharacter, public IPhotographable
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UDiscoveryBookData* DiscoveryData;
	
public:
	// Sets default values for this character's properties
	ABaseAnimalCharacter();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	virtual UDiscoveryBookData* GetDiscoveryBookData_Implementation() const override;
	virtual bool AddNewDataToDiscoveryBook_Implementation() const override;
};
