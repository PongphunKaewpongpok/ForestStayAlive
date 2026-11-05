// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"

#include "Kismet/GameplayStatics.h"



const FString UMySaveGame::SaveSlotName = TEXT("SaveGame");

UMySaveGame* UMySaveGame::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		UMySaveGame* LoadedSave = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

		if (IsValid(LoadedSave))
		{
			return LoadedSave;
		}
	}

	UMySaveGame* NewSave = Cast<UMySaveGame>(
		UGameplayStatics::CreateSaveGameObject(
			UMySaveGame::StaticClass()
		)
	);

	if (IsValid(NewSave))
	{
		NewSave->SaveGame();
	}

	return NewSave;
}

bool UMySaveGame::SaveGame()
{
	return UGameplayStatics::SaveGameToSlot(
		this,
		SaveSlotName,
		0
	);
}

void UMySaveGame::AddPhotoData(const FString& Name, const FString& FilePath)
{
	FPhotoData& Data = PhotosData.FindOrAdd(Name);

	Data.FilePaths.AddUnique(FilePath);

	SaveGame();
}

const FPhotoData* UMySaveGame::FindPhotoData(const FString& Name) const
{
	return PhotosData.Find(Name);
}