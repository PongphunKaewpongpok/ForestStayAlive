// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlaySound3D.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_PlaySound3D::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;
	
	const FVector SoundLocation = MeshComp->GetComponentLocation();
	UGameplayStatics::PlaySoundAtLocation(
		MeshComp->GetWorld(),
		Sound,
		SoundLocation,
		VolumeMultiplier,
		PitchMultiplier,
		0.0f,
		CustomAttenuation
	);
}