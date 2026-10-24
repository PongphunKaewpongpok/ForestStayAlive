// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnObjectData.generated.h"


USTRUCT(BlueprintType)
struct FSpawnObjectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UStaticMesh>> Meshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Chance = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAlignToSurface = false;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinScale = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxScale = 1.2f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinHeight = -100000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHeight = 100000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinSlope = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSlope = 30.f;
};



class FSpawnObjectLibrary
{
public:
	static float GetTotalChance(const TArray<FSpawnObjectData>& SpawnObjects);

	static FSpawnObjectData Tree();
	static FSpawnObjectData Rock();
	static FSpawnObjectData Bush();
	
	static TArray<FSpawnObjectData> GetSpawnObjects_Forest();
};