// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "StructureData.h"

#include "StructureGenerator.generated.h"


/**
 * 
 */
UCLASS()
class FORESTSTAYALIVE_API UStructureGenerator : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const int32 InSeed, const int InRenderDistance, const float InChunkSize);

	void GenerateStructure(FIntPoint ChunkCoord);

private:
	int32 Seed;
	int8 RenderDistance;
	float ChunkSize;
	
	TMap<FIntPoint, UStructureData> StructuresArray;
};