// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraComponent.h"

#include "ForestStayAlive/DiscoveryBookManager.h"
#include "ForestStayAlive/MySaveGame.h"
#include "Misc/DateTime.h"
#include "ForestStayAlive/Interfaces/Photographable.h"
#include "ForestStayAlive/DataAssets/DiscoveryBookData.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Misc/DateTime.h"

// Sets default values for this component's properties
UMyCameraComponent::UMyCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyCameraComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UMyCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




void UMyCameraComponent::TakeAPhoto()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!IsValid(Character)) return;
	
	
	FHitResult HitResult;

	const FVector Start = Character->GetActorLocation();
	const FVector End = Start + Character->GetActorForwardVector() * 10000.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	FString PhotoName = TEXT("Screenshot");
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();

		if (IsValid(HitActor) && HitActor->Implements<UPhotographable>())
		{
			UDiscoveryBookData* DiscoveryData = IPhotographable::Execute_GetDiscoveryBookData(HitActor);

			if (IsValid(DiscoveryData))
			{
				PhotoName = DiscoveryData->DiscoveryID.ToString();
				const bool bResult = IPhotographable::Execute_AddNewDataToDiscoveryBook(HitActor);
			}
		}
	}

	
	
	const FString FileName = FDateTime::Now().ToString(TEXT("Capture_%Y%m%d_%H%M%S.png"));

	FScreenshotRequest::RequestScreenshot(
		FileName,
		false,
		false,
		false
	);

	UDiscoveryBookManager* Manager = GetWorld()->GetGameInstance()->GetSubsystem<UDiscoveryBookManager>();

	if (!IsValid(Manager)) return;

	UMySaveGame* SaveGame = Manager->GetSaveData();

	if (!IsValid(SaveGame)) return;

	SaveGame->AddPhotoData(PhotoName, FileName);
	SaveGame->SaveGame();
}