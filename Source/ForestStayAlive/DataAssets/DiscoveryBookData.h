// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DiscoveryBookData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FORESTSTAYALIVE_API UDiscoveryBookData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DiscoveryID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name = FText::FromString("Unknown");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, FText> Appearances;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, FText> Benefits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FText> Precautions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FText> FoundInLocations;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UStaticMesh>> MeshesList;

	
};