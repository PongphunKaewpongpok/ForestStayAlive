// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ForestStayAliveCharacter.generated.h"

class AWeretigerCharacter;
class UCameraComponent;
class UMainUIWidget;
class UAttributeComponent;

UCLASS()
class FORESTSTAYALIVE_API AForestStayAliveCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY()
	UCameraComponent* MyFirstPersonCamera;
	
protected:
    UPROPERTY(BlueprintReadWrite)
	float TargetFOV = 70.0f;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* DeathAnimation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Footstep")
	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> FootstepSounds;
	UPROPERTY(EditDefaultsOnly, Category = "Footstep")
	TObjectPtr<USoundBase> DefaultFootstepSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundAttenuation> FootstepSoundAttenuation;
	UPROPERTY(EditDefaultsOnly, Category = "Footstep")
	float RunNoiseLoudness = 1.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Footstep")
	float RunNoiseRange = 1800.f;
	
public:
	// Sets default values for this character's properties
	AForestStayAliveCharacter();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	
	
	void GotExecuted(AWeretigerCharacter* Weretiger);
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category="Jump")
	bool bCanJump = true;
	
	UFUNCTION(BlueprintCallable, Category="Jump")
	void SetCanJump(bool CanJump);
	
	virtual void Jump() override;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeComponent> AttributeComponent;
	
	void SetTargetFOV(float FOV);
	void SetCameraFOV(const float FOV) const;
};
