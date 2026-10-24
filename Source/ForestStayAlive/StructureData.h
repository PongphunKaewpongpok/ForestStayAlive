// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructureData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStructureData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Type = TEXT("None");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;
;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UStaticMesh>> Meshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 5;
	
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




class FStructureLibrary
{
public:
	/*static FStructureLibrary Tree();
	static FStructureLibrary Rock();
	static FStructureLibrary Bush();
	
	static TArray<FStructureLibrary> GetSpawnObjects_Forest();*/
};