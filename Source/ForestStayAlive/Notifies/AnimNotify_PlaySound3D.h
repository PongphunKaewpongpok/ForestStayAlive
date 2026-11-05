// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/SoundAttenuation.h"

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "AnimNotify_PlaySound3D.generated.h"

/**
 * 
 */
UCLASS()
class FORESTSTAYALIVE_API UAnimNotify_PlaySound3D : public UAnimNotify_PlaySound
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundAttenuation> CustomAttenuation;

	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;
};
