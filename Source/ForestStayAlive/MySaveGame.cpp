// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"

#include "Kismet/GameplayStatics.h"


TMap<FString, FString> UMySaveGame::GetPhotosData()
{
	return PhotosData;
}

void UMySaveGame::AddPhotoData(const FString& Name, const FString& FilePath)
{
	PhotosData.Add(Name, FilePath);
}

bool UMySaveGame::SaveGame()
{
	return UGameplayStatics::SaveGameToSlot(
		this,
		TEXT("SaveData"),
		0
	);
}
