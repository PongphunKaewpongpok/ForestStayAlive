// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DiscoveryBookSettings.generated.h"

class UDiscoveryBookData;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class FORESTSTAYALIVE_API UDiscoveryBookSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Discovery Book")
	TArray<TSoftObjectPtr<UDiscoveryBookData>> PlantsDiscoveryDataAsset;
	UPROPERTY(Config, EditAnywhere, Category = "Discovery Book")
	TArray<TSoftObjectPtr<UDiscoveryBookData>> AnimalsDiscoveryDataAsset;
};
