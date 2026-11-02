// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"

#include "Kismet/GameplayStatics.h"


TMap<FString, FPhotoData> UMySaveGame::GetPhotosData() const
{
	return PhotosData;
}

void UMySaveGame::AddPhotoData(const FString& Name, const FString& FilePath)
{
	if (const FPhotoData* Data = PhotosData.Find(Name))
	{
		for (const FString& Path : Data->FilePaths)
		{
			UE_LOG(LogTemp, Log, TEXT("%s"), *Path);
		}
	}
}

bool UMySaveGame::SaveGame()
{
	return UGameplayStatics::SaveGameToSlot(
		this,
		TEXT("SaveData"),
		0
	);
}
