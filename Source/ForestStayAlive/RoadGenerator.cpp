#include "RoadGenerator.h"

#include "FastNoiseLite.h"


void URoadGenerator::Initialize(const int32 InSeed, const float InRoadWidth, const int InRenderDistance, const int InRoadPointsPerChunk, const float InChunkSize)
{
    Seed = InSeed;
    RoadWidth = InRoadWidth;
    RenderDistance = InRenderDistance;
    RoadPointsPerChunk = InRoadPointsPerChunk;
    ChunkSize = InChunkSize;
    DistancePoint = InChunkSize/(InRoadPointsPerChunk-1);
    
    RoadPositivePoints.Empty();
    RoadNegativePoints.Empty();
}



void URoadGenerator::GenerateRoad()
{
    RoadPositivePoints.Empty();
    RoadNegativePoints.Empty();
    
    GeneratedMinChunkX = -RenderDistance;
    GeneratedMaxChunkX = RenderDistance;
    
    GenerateRoad(true, RenderDistance * RoadPointsPerChunk);
    GenerateRoad(false, RenderDistance * RoadPointsPerChunk);
}

void URoadGenerator::GenerateRoad(const bool bPositive, const int32 Count)
{
    FastNoiseLite RoadNoise;

    RoadNoise.SetSeed(Seed + (bPositive ? 0 : 9999));
    RoadNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    TArray<FRoadData>& RoadArray = (bPositive) ? RoadPositivePoints : RoadNegativePoints;

    for (int32 i = 0; i < Count; ++i)
    {
        FVector Position;

        if (RoadArray.Num() == 0)
        {
            Position = FVector::ZeroVector;
        }
        else
        {
            const FRoadData& Previous = RoadArray.Last();

            Position.X = Previous.Position.X + (bPositive ? 1 : -1) * DistancePoint;

            float DeltaY = RoadNoise.GetNoise(Position.X * RoadFrequency, 0.0) * RoadAmplitude;
            Position.Y = Previous.Position.Y + DeltaY;
            
            Position.Z = 0.0f;
        }

        RoadArray.Add(FRoadData(Position, RoadWidth));
    }
}

void URoadGenerator::UpdateAutoGenerateRoadPoints(const FIntPoint PlayerChunk)
{
    const int AddChunkCount = 5;
    while (PlayerChunk.X <= GeneratedMinChunkX+RenderDistance)
    {
        GeneratedMinChunkX -= AddChunkCount;
        GenerateRoad(false, AddChunkCount * RoadPointsPerChunk);
    }
    
    while (PlayerChunk.X >= GeneratedMaxChunkX-RenderDistance)
    {
        GeneratedMaxChunkX += AddChunkCount;
        GenerateRoad(true, AddChunkCount * RoadPointsPerChunk);
    }
}


bool URoadGenerator::HasRoadInChunk(const FIntPoint ChunkCoord) const
{
    return GetRoadDataInChunk(ChunkCoord).Num() > 1;
}

TArray<FRoadData> URoadGenerator::GetRoadDataInChunk(const FIntPoint ChunkCoord) const
{
    TArray<FRoadData> Result;

    const float MinX = ChunkCoord.X * ChunkSize;
    const float MaxX = MinX + ChunkSize;
    
    const float MinY = ChunkCoord.Y * ChunkSize;
    const float MaxY = MinY + ChunkSize;
    
    if (MaxX >= 0)
    {
        int32 StartIndex =
            FMath::Max(
                0,
                FMath::FloorToInt(FMath::Max(MinX, 0.f) / DistancePoint)
            );

        int32 EndIndex =
            FMath::Min(
                RoadPositivePoints.Num() - 1,
                FMath::CeilToInt(MaxX / DistancePoint)
            );

        for (int32 i = StartIndex; i <= EndIndex; ++i)
        {
            if (RoadPositivePoints[i].Position.X >= MinX &&
                RoadPositivePoints[i].Position.X <= MaxX &&
                RoadPositivePoints[i].Position.Y >= MinY - RoadWidth &&
                RoadPositivePoints[i].Position.Y <= MaxY + RoadWidth)
            {
                Result.Add(RoadPositivePoints[i]);
            }
        }
    }

    if (MinX <= 0)
    {
        int32 StartIndex =
            FMath::Max(
                0,
                FMath::FloorToInt(FMath::Max(-MaxX, 0.f) / DistancePoint)
            );

        int32 EndIndex =
            FMath::Min(
                RoadNegativePoints.Num() - 1,
                FMath::CeilToInt(-MinX / DistancePoint)
            );

        for (int32 i = StartIndex; i <= EndIndex; ++i)
        {
            if (RoadNegativePoints[i].Position.X >= MinX &&
                RoadNegativePoints[i].Position.X <= MaxX &&
                RoadNegativePoints[i].Position.Y >= MinY - RoadWidth &&
                RoadNegativePoints[i].Position.Y <= MaxY + RoadWidth)
            {
                Result.Add(RoadNegativePoints[i]);
            }
        }
    }

    return Result;
}

bool URoadGenerator::IsRoadCollision(const FVector& WorldPosition, const FIntPoint& ChunkCoord, const float Radius) const
{
    /*TArray<FRoadData> RoadPoints = GetRoadDataInChunk(ChunkCoord);*/
    
    TArray<FRoadData> RoadPoints;

    RoadPoints.Append(RoadPositivePoints);
    RoadPoints.Append(RoadNegativePoints);
    
    for (int32 i = 0; i < RoadPoints.Num() - 1; i++)
    {
        const FVector Start = RoadPoints[i].Position;
        const FVector End = RoadPoints[i + 1].Position;

        FVector Closest = FMath::ClosestPointOnSegment(WorldPosition, Start, End);

        const float Distance = FVector::Dist2D(WorldPosition, Closest);
        if (Distance <= RoadWidth/2 + Radius) { return true; }
    }

    return false;
}