// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


class AForestStayAlivePlayerController;
class UMainUIWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORESTSTAYALIVE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UAttributeComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float Health = 100.0f;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyDamage(const float Amount);
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(const float Amount);
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;
	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float MaxStamina = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float Stamina = 100.0f;
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void ModifyStamina(const float Amount);
	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStaminaPercent() const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprint")
	float WalkSpeed = 300.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprint")
	float RunSpeed = 1000.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Sprint")
	bool bIsSprinting = false;
	UFUNCTION(BlueprintPure, Category = "Sprint")
	bool IsSprinting();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StartSprint();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprint();
	UFUNCTION(BlueprintPure, Category = "Sprint")
	float GetWalkSpeed() const;
	UFUNCTION(BlueprintPure, Category = "Sprint")
	float GetRunSpeed() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hunger")
	float MaxHunger = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hunger")
	float Hunger = 100.0f;
	UFUNCTION(BlueprintCallable, Category = "Hunger")
	void ModifyHunger(const float Amount);
	UFUNCTION(BlueprintPure, Category = "Hunger")
	float GetHungerPercent() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirst")
	float MaxThirst = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Thirst")
	float Thirst = 100.0f;
	UFUNCTION(BlueprintCallable, Category = "Thirst")
	void ModifyThirst(const float Amount);
	UFUNCTION(BlueprintPure, Category = "Thirst")
	float GetThirstPercent() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sanity")
	float MaxSanity = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sanity")
	float Sanity = 100.0f;
	UFUNCTION(BlueprintCallable, Category = "Sanity")
	void ModifySanity(const float Amount);
	UFUNCTION(BlueprintPure, Category = "Sanity")
	float GetSanityPercent() const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BodyTemperature")
	float BodyTemperature = 37.0f;

protected:

public:	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
	UPROPERTY(VisibleAnywhere)
	AForestStayAlivePlayerController* OwnPlayerController;
};
