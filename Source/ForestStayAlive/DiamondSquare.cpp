// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondSquare.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

#include "FastNoiseLite.h"
#include "SpawnObjectData.h"
#include "RoadGenerator.h"
#include "ChunkManager.h"

// Sets default values
/*

0 = Terrain
1 = Water
2 = Road

*/
ADiamondSquare::ADiamondSquare()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	RootComponent = ProceduralMesh;
}

void ADiamondSquare::BeginPlay()
{
	Super::BeginPlay();
}

void ADiamondSquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



int32 ADiamondSquare::GetChunkSeed(const int32 Salt) const
{
	return Seed ^
		(ChunkCoord.X * 73856093) ^
		(ChunkCoord.Y * 19349663) ^
		Salt;
}


void ADiamondSquare::CreateVertices()
{

	HeightMap.Reset();


	FastNoiseLite Noise;

	Noise.SetSeed(Seed);

	Noise.SetNoiseType(
		FastNoiseLite::NoiseType_Perlin
	);



	for(int x = 0; x <= VertexCount; x++)
	{
		for(int y = 0; y <= VertexCount; y++)
		{

			int WorldX =
				ChunkCoord.X * VertexCount + x;


			int WorldY =
				ChunkCoord.Y * VertexCount + y;



			float Height =
				Noise.GetNoise(
					WorldX * NoiseScale,
					WorldY * NoiseScale
				)
				*
				ZMultiplier;



			HeightMap.Add(Height);



			Vertices.Add(
				FVector(
					x * Scale,
					y * Scale,
					Height
				)
			);


			UV0.Add(
				FVector2D(
					x * UVScale,
					y * UVScale
				)
			);

		}
	}

}

void ADiamondSquare::CreateTriangles()
{
	int Vertex = 0;
	
	for (int x = 0; x < VertexCount; ++x)
	{
		for (int y = 0; y < VertexCount; ++y)
		{
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + VertexCount + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + VertexCount + 2);
			Triangles.Add(Vertex + VertexCount + 1);
		
			++Vertex;
		}
		++Vertex;
	}
}

void ADiamondSquare::GenerateTerrain()
{

	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();
	Normals.Reset();
	Tangents.Reset();
	
	HeightMap.Reset();


	// Generation
	CreateVertices();
	if (bHasRiver) { GenerateRiver(); }
	CreateTriangles();
	GenerateWaterTerrain();
	if (bHasRoad) { GenerateRoad(); }



	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
		Vertices,
		Triangles,
		UV0,
		Normals,
		Tangents
	);



	ProceduralMesh->CreateMeshSection(
		0,
		Vertices,
		Triangles,
		Normals,
		UV0,
		TArray<FColor>(),
		Tangents,
		true
	);


	ProceduralMesh->SetMaterial(
		0,
		Material
	);

}

void ADiamondSquare::GenerateRiver()
{
	RiverMask.Reset();
	RiverMask.SetNum((VertexCount + 1) * (VertexCount + 1), false);

	FastNoiseLite RiverNoise;

	RiverNoise.SetSeed(Seed + 9999);
	RiverNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

	for(int32 x = 0; x <= VertexCount; x++)
	{
		for(int32 y = 0; y <= VertexCount; y++)
		{
			int32 Index = x * (VertexCount + 1) + y;

			float WorldX =
				(ChunkCoord.X * VertexCount + x) * Scale;

			float WorldY =
				(ChunkCoord.Y * VertexCount + y) * Scale;

			float RiverValue =
				RiverNoise.GetNoise(
					WorldX * RiverScale,
					WorldY * RiverScale
				);

			if(FMath::Abs(RiverValue) < RiverWidth)
			{
				RiverMask[Index] = true;

				float Alpha =
					1.f -
					FMath::Abs(RiverValue) /
					RiverWidth;

				Vertices[Index].Z -= RiverDepth * Alpha;
			}
		}
	}
}

void ADiamondSquare::GenerateWaterTerrain()
{
    WaterVertices.Reset();
    WaterTriangles.Reset();
    WaterUV.Reset();
    WaterNormals.Reset();
    WaterTangents.Reset();

	WaterVertices.Append(Vertices);

	bool hWater = false;
	for (FVector& Vertex : WaterVertices)
	{
		if (Vertex.Z <= WaterLevel)
		{
			hWater = true;
		}
		Vertex.Z = WaterLevel;
	}
	
	if (!hWater) { return; }

	WaterTriangles.Append(Triangles);
	WaterUV.Append(UV0);

    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
        WaterVertices,
        WaterTriangles,
        WaterUV,
        WaterNormals,
        WaterTangents
    );

    ProceduralMesh->CreateMeshSection(
        1,
        WaterVertices,
        WaterTriangles,
        WaterNormals,
        WaterUV,
        TArray<FColor>(),
        WaterTangents,
        false
    );
    ProceduralMesh->SetMaterial(1, WaterMaterial);
}

void ADiamondSquare::GenerateStructures()
{
	
}

void ADiamondSquare::GenerateObject(const TArray<FSpawnObjectData>& SpawnObjects, const int SpawnCount)
{
	FRandomStream Stream;
	const int32 ChunkSeed = GetChunkSeed(100);
	Stream.Initialize(ChunkSeed);
	
	
	const int32 TotalChance = FSpawnObjectLibrary::GetTotalChance(SpawnObjects);
	
	for (int32 _ = 0; _ < SpawnCount; _++)
	{
		float Random = Stream.FRandRange(0.f, TotalChance);

		const FSpawnObjectData* SelectedData = nullptr;

		float Current = 0.f;
		for (const FSpawnObjectData& Data : SpawnObjects)
		{
			Current += Data.Chance;

			if (Random <= Current)
			{
				SelectedData = &Data;
				break;
			}
		}

		if (!SelectedData) { continue; }
		if (SelectedData->Meshes.IsEmpty()) { continue; }
		
		
		
		for (int32 i = 0; i < SelectedData->Count; i++)
		{
			int32 X = Stream.RandRange(0, VertexCount);
			int32 Y = Stream.RandRange(0, VertexCount);

			int32 Index = X + Y * (VertexCount + 1);
			if (!Vertices.IsValidIndex(Index)) { continue; }
		
			if (OwnerChunkManager->RoadGenerator->IsRoadCollision(GetActorLocation()+Vertices[Index], ChunkCoord, 100.0f)) { continue; }
		
			const float Height = Vertices[Index].Z;
			if (Height < SelectedData->MinHeight || Height > SelectedData->MaxHeight) { continue; }

		
			const float Slope =
				FMath::RadiansToDegrees(
					FMath::Acos(FMath::Clamp(Normals[Index].Z, -1.f, 1.f))
				);
			if (Slope < SelectedData->MinSlope || Slope > SelectedData->MaxSlope) { continue; }

		
			FVector Position = Vertices[Index];
			FRotator Rotation = FRotator::ZeroRotator;
			Rotation.Yaw = Stream.FRandRange(0.f, 360.f);


			const float ObjectScale = Stream.FRandRange(SelectedData->MinScale, SelectedData->MaxScale);
		
			FTransform Transform;
			Transform.SetLocation(Position);
			Transform.SetRotation(Rotation.Quaternion());
			Transform.SetScale3D(FVector(ObjectScale));
		
		
			int32 MeshIndex = Stream.RandRange(0, SelectedData->Meshes.Num() - 1);
			UStaticMesh* Mesh = SelectedData->Meshes[MeshIndex];
			UHierarchicalInstancedStaticMeshComponent* HISM = GetOrCreateHISM(Mesh);
		
			if (!HISM) { continue; }
			HISM->AddInstance(Transform);
		}
	}
}

void ADiamondSquare::GenerateRoad()
{
	if (!OwnerChunkManager) { return; }
	if (!OwnerChunkManager->RoadGenerator) { return; }
	
	const float ChunkSize = Scale * VertexCount;
	if (!OwnerChunkManager->RoadGenerator->HasRoadInChunk(ChunkCoord)) { return; }
	
	TArray<FRoadData> RoadPoints = OwnerChunkManager->RoadGenerator->GetRoadDataInChunk(ChunkCoord);
	
	if (RoadPoints.Num() < 2) { return; }



	TArray<FVector> RoadVertices;
	TArray<int32> RoadTriangles;
	TArray<FVector> RoadNormals;
	TArray<FVector2D> RoadUVs;
	TArray<FProcMeshTangent> RoadTangents;


	for(int32 i = 0; i < RoadPoints.Num(); i++)
	{

		FVector WorldPos = RoadPoints[i].Position;
		/*WorldPos.Z += 25.0;*/
		FVector Direction;
		
		if (i == 0)
		{
			Direction = RoadPoints[i+1].Position - WorldPos;
		}
		else if (i == RoadPoints.Num()-1)
		{
			Direction = WorldPos - RoadPoints[i-1].Position;
		}
		else
		{
			Direction = RoadPoints[i+1].Position - RoadPoints[i-1].Position;
		}
		Direction.Normalize();

		
		// Vector ด้านข้างถนน
		FVector Right = FVector::CrossProduct(FVector::UpVector, Direction);
		Right.Normalize();
		
		
		// World Position ของขอบถนน
		FVector LeftWorld = WorldPos - Right * (RoadWidth/2);
		FVector RightWorld = WorldPos + Right * (RoadWidth/2);


		/* World -> Chunk Local*/
		FVector ChunkOrigin;

		ChunkOrigin.X = ChunkCoord.X * ChunkSize;
		ChunkOrigin.Y = ChunkCoord.Y * ChunkSize;
		ChunkOrigin.Z = 0;

		FVector LeftLocal = LeftWorld - ChunkOrigin;
		FVector RightLocal = RightWorld - ChunkOrigin;
		

		LeftLocal.Z = GetTerrainHeight(LeftWorld.X, LeftWorld.Y) + 5.0f;
		RightLocal.Z = GetTerrainHeight(RightWorld.X, RightWorld.Y) + 5.0f;

		RoadVertices.Add(LeftLocal);
		RoadVertices.Add(RightLocal);
		
		const float UVY = i * 0.05f;
		
		RoadUVs.Add(FVector2D(0,UVY));
		RoadUVs.Add(FVector2D(1,UVY));
	}
	


	for(int32 i=0; i < RoadVertices.Num()-2; i += 2)
	{
		RoadTriangles.Add(i);
		RoadTriangles.Add(i+1);
		RoadTriangles.Add(i+2);

		RoadTriangles.Add(i+1);
		RoadTriangles.Add(i+3);
		RoadTriangles.Add(i+2);
	}
	
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
		RoadVertices,
		RoadTriangles,
		RoadUVs,
		RoadNormals,
		RoadTangents
	);
	ProceduralMesh->CreateMeshSection(
		2,
		RoadVertices,
		RoadTriangles,
		RoadNormals,
		RoadUVs,
		TArray<FColor>(),
		RoadTangents,
		true
	);
}

float ADiamondSquare::GetTerrainHeight(float WorldX, float WorldY)
{
	const float ChunkSize = Scale * VertexCount;
	
	const float LocalX = (WorldX - ChunkCoord.X * ChunkSize) / Scale;
	const float LocalY = (WorldY - ChunkCoord.Y * ChunkSize) / Scale;
	
	int X =
		FMath::Clamp(
			FMath::RoundToInt(LocalX),
			0,
			VertexCount
		);
	
	int Y =
		FMath::Clamp(
			FMath::RoundToInt(LocalY),
			0,
			VertexCount
		);
	
	int Index = X * (VertexCount + 1) + Y;
	
	if(HeightMap.IsValidIndex(Index))
	{
		return HeightMap[Index];
	}
	
	return 0.f;

}

UHierarchicalInstancedStaticMeshComponent* ADiamondSquare::GetOrCreateHISM(UStaticMesh* Mesh)
{
	if (!Mesh) { return nullptr; }

	if (UHierarchicalInstancedStaticMeshComponent** Found = HISMMap.Find(Mesh))
	{
		return *Found;
	}

	UHierarchicalInstancedStaticMeshComponent* NewHISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);

	NewHISM->SetupAttachment(ProceduralMesh);
	NewHISM->RegisterComponent();
	NewHISM->SetStaticMesh(Mesh);

	HISMMap.Add(Mesh, NewHISM);

	return NewHISM;
}