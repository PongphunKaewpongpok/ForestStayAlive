// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

struct FDiscoveryBookEntryProgress;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FPhotoData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	TArray<FString> FilePaths;
};

UCLASS()
class FORESTSTAYALIVE_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	TMap<FString, FPhotoData> PhotosData;

public:
    static const FString SaveSlotName;
    static UMySaveGame* LoadSaveGame();
    bool SaveGame();

    void AddPhotoData(
        const FString& Name,
        const FString& FilePath
    );

    const FPhotoData* FindPhotoData(const FString& Name) const;
    const TMap<FString, FPhotoData>& GetPhotosData() const
    {
        return PhotosData;
    }

    UPROPERTY()
    TArray<FDiscoveryBookEntryProgress> Entries;
};