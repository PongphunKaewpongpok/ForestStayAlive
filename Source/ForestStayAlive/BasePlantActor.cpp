// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlantActor.h"

#include "DiscoveryBookManager.h"
#include "HighlightableObject.h"
#include "DataAssets/DiscoveryBookData.h"

// Sets default values
ABasePlantActor::ABasePlantActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ABasePlantActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HighlightableObjectClass)
	{
		HighlightableObject = NewObject<UHighlightableObject>(
			this,
			HighlightableObjectClass
		);

		if (HighlightableObject) HighlightableObject->RegisterComponent();
	}
}

// Called every frame
void ABasePlantActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UDiscoveryBookData* ABasePlantActor::GetDiscoveryBookData_Implementation() const
{
	return DiscoveryData;
}

bool ABasePlantActor::AddNewDataToDiscoveryBook_Implementation() const
{
	if (!IsValid(DiscoveryData)) return false;

	UGameInstance* GameInstance = GetGameInstance();
	if (!IsValid(GameInstance)) return false;

	UDiscoveryBookManager* DiscoveryManager = GameInstance->GetSubsystem<UDiscoveryBookManager>();
	if (!IsValid(DiscoveryManager)) return false;

	DiscoveryManager->UnlockDiscoveryName(DiscoveryData->DiscoveryID);

	return true;
}