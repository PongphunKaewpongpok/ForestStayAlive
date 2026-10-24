#pragma once

#include "CoreMinimal.h"
#include "RoadGenerator.generated.h"


USTRUCT()
struct FRoadData
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Position;

	UPROPERTY()
	float Width;


	FRoadData()
	{
		Position = FVector::ZeroVector;
		Width = 800.f;
	}

	FRoadData(FVector InPosition, float InWidth)
	{
		Position = InPosition;
		Width = InWidth;
	}
};



UCLASS()
class FORESTSTAYALIVE_API URoadGenerator : public UObject
{
	GENERATED_BODY()


public:
	void Initialize(const int32 InSeed, const float InRoadWidth, const int InRenderDistance, const int InSegments, const float InChunkSize);
	
	void GenerateRoad();
	void GenerateRoad(const bool bPositive, const int32 Count);
	void UpdateAutoGenerateRoadPoints(const FIntPoint PlayerChunk);
	
	bool HasRoadInChunk( const FIntPoint ChunkCoord) const;
	TArray<FRoadData> GetRoadDataInChunk(const FIntPoint ChunkCoord) const;
	bool IsRoadCollision(const FVector& WorldPosition, const FIntPoint& ChunkCoord, const float Radius) const;




private:
	int32 Seed;
	int8 RenderDistance;
	
	UPROPERTY(Meta = ( ClampMin = 2 ))
	int8 RoadPointsPerChunk = 2;
	float DistancePoint;
	
	int32 GeneratedMinChunkX = 0;
	int32 GeneratedMaxChunkX = 0;
	TArray<FRoadData> RoadPositivePoints;
	TArray<FRoadData> RoadNegativePoints;


	float RoadY = 0.0f;
	float RoadWidth = 800.0f;
	float ChunkSize = 1000.0f;
	
	double RoadFrequency = 0.05f;
	float RoadAmplitude = 20.0f;
	
};