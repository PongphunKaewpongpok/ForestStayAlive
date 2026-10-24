// Fill out your copyright notice in the Description page of Project Settings.


#include "StructureGenerator.h"

void UStructureGenerator::Initialize(const int32 InSeed, const int InRenderDistance, const float InChunkSize)
{
	Seed = InSeed;
	RenderDistance = InRenderDistance;
	ChunkSize = InChunkSize;
}