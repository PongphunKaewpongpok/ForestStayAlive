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

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	TMap<FString, FPhotoData> GetPhotosData() const;

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void AddPhotoData(const FString& Name, const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	bool SaveGame();

	// Discovery Book Progress
	UPROPERTY(SaveGame, BlueprintReadWrite, Category = "Discovery Book")
	TArray<FDiscoveryBookEntryProgress> Entries;
};