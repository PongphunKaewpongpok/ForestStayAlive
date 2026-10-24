// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FORESTSTAYALIVE_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	TMap<FString, FString> GetPhotosData();
	void AddPhotoData(const FString& Name, const FString& FilePath);
	bool SaveGame();
	
private:
	UPROPERTY()
	TMap<FString, FString> PhotosData;
};
