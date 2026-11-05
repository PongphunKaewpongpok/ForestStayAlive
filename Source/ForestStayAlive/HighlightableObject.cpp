// Fill out your copyright notice in the Description page of Project Settings.


#include "HighlightableObject.h"

// Sets default values for this component's properties
UHighlightableObject::UHighlightableObject()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called every frame
void UHighlightableObject::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHighlightableObject::Highlight(const bool Status) const
{
	if (!GetOwner() || !HighlightMaterial) return;
	
	TArray<UStaticMeshComponent*> MeshComponents;
	GetOwner()->GetComponents<UStaticMeshComponent>(MeshComponents);

	for (UStaticMeshComponent* Mesh : MeshComponents)
	{
		if (!Mesh) continue;

		Mesh->SetOverlayMaterial(Status ? HighlightMaterial : nullptr);
	}
}

