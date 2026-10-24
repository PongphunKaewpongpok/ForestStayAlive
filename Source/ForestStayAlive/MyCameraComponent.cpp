// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraComponent.h"

#include "MySaveGame.h"

#include "Kismet/GameplayStatics.h"
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
	// Save To Folder
	const FString FileName = FDateTime::Now().ToString(TEXT("Capture_%Y%m%d_%H%M%S.png"));
	
	FScreenshotRequest::RequestScreenshot(
		FileName,
		false,
		false,
		false
		);
	
	// Save Game
	UMySaveGame* SaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	
	SaveGame->AddPhotoData(TEXT("Photo01"), FileName);
	SaveGame->SaveGame();
}