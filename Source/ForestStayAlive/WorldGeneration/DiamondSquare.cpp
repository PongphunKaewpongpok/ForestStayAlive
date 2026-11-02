// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondSquare.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

#include "FastNoiseLite.h"
#include "RoadGenerator.h"
#include "ChunkManager.h"
#include "ForestStayAlive/DataASsets/MeshSpawnData.h"
#include "ForestStayAlive/DataASsets/StructureData.h"


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
	Noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);



	for(int x = 0; x <= VertexCount; x++)
	{
		for(int y = 0; y <= VertexCount; y++)
		{

			int WorldX = ChunkCoord.X * VertexCount + x;
			int WorldY = ChunkCoord.Y * VertexCount + y;

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
	
	SpawnedPositions.Reset();


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

			float WorldX = (ChunkCoord.X * VertexCount + x) * Scale;
			float WorldY = (ChunkCoord.Y * VertexCount + y) * Scale;

			float RiverValue =
				RiverNoise.GetNoise(
					WorldX * RiverScale,
					WorldY * RiverScale
				);

			if(FMath::Abs(RiverValue) < RiverWidth)
			{
				RiverMask[Index] = true;

				float Alpha = 1.0f - FMath::Abs(RiverValue)/RiverWidth;

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
	
	if (!hWater) return;

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

void ADiamondSquare::GenerateStructures(const TArray<UStructureData*>& StructuresList)
{
	if (StructuresList.IsEmpty()) return;
	
	FRandomStream Stream;
	const int32 ChunkSeed = GetChunkSeed(100);
	Stream.Initialize(ChunkSeed);
	
	const int32 Index = Stream.RandRange(0, StructuresList.Num()-1);
	
	UStructureData* StructureData = StructuresList[Index];
	if (!StructureData) return;
	
	if (Stream.FRand() > StructuresList[Index]->Chance) return;
	
	GenerateObject(StructureData);
}





void ADiamondSquare::GenerateObject(const UStructureData* StructureData)
{
    if (!IsValid(StructureData)) return;

    FRandomStream Stream;
    const int32 ChunkSeed = GetChunkSeed(100);
    Stream.Initialize(ChunkSeed);

    const float MaxChunkLength = VertexCount * Scale;
    const FTransform ActorTransform = GetActorTransform();

    FVector CenterPosition = FVector::ZeroVector;
    FVector CenterWorldPosition = FVector::ZeroVector;

    bool bCenterSpawned = false;

    // =================================================
    // Random Spawn Position & Check Terrain
    // =================================================

    auto TryGetSpawnPosition =
        [&](const UMeshSpawnData* MeshData,
            bool bUseCenter,
            float MinRadius,
            float MaxRadius,
            FVector& OutPosition,
            FVector& OutWorldPosition) -> bool
    {
        if (!IsValid(MeshData) || Scale <= 0.f) return false;

        float LocalX;
        float LocalY;

        if (bUseCenter)
        {
            MinRadius = FMath::Max(0.f, MinRadius);
            MaxRadius = FMath::Max(MinRadius, MaxRadius);

            const float Angle = Stream.FRandRange(0.f, 360.f);

            
            const float Radius = FMath::Sqrt(
                Stream.FRandRange(
                    FMath::Square(MinRadius),
                    FMath::Square(MaxRadius)
                )
            );

            LocalX = CenterPosition.X + FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius;
            LocalY = CenterPosition.Y + FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius;
        }
        else
        {
            LocalX = Stream.FRandRange(0.f, MaxChunkLength);
            LocalY = Stream.FRandRange(0.f, MaxChunkLength);
        }

        
        if (LocalX < 0.f || LocalX > MaxChunkLength || LocalY < 0.f || LocalY > MaxChunkLength) return false;

        
        const float GridX = LocalX / Scale;
        const float GridY = LocalY / Scale;

        int32 X0 = FMath::FloorToInt(GridX);
        int32 Y0 = FMath::FloorToInt(GridY);

        X0 = FMath::Clamp(X0, 0, VertexCount - 1);
        Y0 = FMath::Clamp(Y0, 0, VertexCount - 1);

        const int32 X1 = X0 + 1;
        const int32 Y1 = Y0 + 1;

        const float FracX = GridX - X0;
        const float FracY = GridY - Y0;

        const int32 Index00 = X0 * (VertexCount + 1) + Y0;
        const int32 Index10 = X1 * (VertexCount + 1) + Y0;
        const int32 Index01 = X0 * (VertexCount + 1) + Y1;
        const int32 Index11 = X1 * (VertexCount + 1) + Y1;

        if (!Vertices.IsValidIndex(Index00) ||
            !Vertices.IsValidIndex(Index10) ||
            !Vertices.IsValidIndex(Index01) ||
            !Vertices.IsValidIndex(Index11) ||
            !Normals.IsValidIndex(Index00) ||
            !Normals.IsValidIndex(Index10) ||
            !Normals.IsValidIndex(Index01) ||
            !Normals.IsValidIndex(Index11))
        {
            return false;
        }

        
        const float Height = FMath::Lerp(
            FMath::Lerp(
                Vertices[Index00].Z,
                Vertices[Index10].Z,
                FracX
            ),
            FMath::Lerp(
                Vertices[Index01].Z,
                Vertices[Index11].Z,
                FracX
            ),
            FracY
        );

        if (Height < MeshData->MinHeight || Height > MeshData->MaxHeight) return false;

        
        const FVector InterpolatedNormal = FMath::Lerp(
            FMath::Lerp(
                Normals[Index00],
                Normals[Index10],
                FracX
            ),
            FMath::Lerp(
                Normals[Index01],
                Normals[Index11],
                FracX
            ),
            FracY
        ).GetSafeNormal();

        
        const float Slope = FMath::RadiansToDegrees(
            FMath::Acos(
                FMath::Clamp(
                    InterpolatedNormal.Z,
                    -1.f,
                    1.f
                )
            )
        );

        if (Slope < MeshData->MinSlope || Slope > MeshData->MaxSlope) return false;

        
        OutPosition = FVector(LocalX, LocalY, Height);
        OutWorldPosition = ActorTransform.TransformPosition(OutPosition);

        
        if (OwnerChunkManager &&
            OwnerChunkManager->RoadGenerator &&
            OwnerChunkManager->RoadGenerator->IsRoadCollision(
                OutWorldPosition,
                ChunkCoord,
                100.f
            ))
        {
            return false;
        }

        return true;
    };

    // =================================================
    // Spawn MeshData
    // =================================================

    auto SpawnMeshData =
        [&](const UMeshSpawnData* MeshData,
            bool bUseCenter,
            float MinRadius,
            float MaxRadius,
            bool bFaceToCenter)
    {
        if (!IsValid(MeshData) || MeshData->Meshes.IsEmpty()) return;

        const int32 Count = Stream.RandRange(MeshData->MinCount, MeshData->MaxCount);

        int32 CurrentSpawnCount = 0;

        const int32 MaxGlobalAttempts = Count * 30;
        int32 GlobalAttempts = 0;

        while (CurrentSpawnCount < Count && GlobalAttempts < MaxGlobalAttempts)
        {
            ++GlobalAttempts;

            if (CurrentSpawnCount >= MeshData->MinCount)
            {
                if (Stream.FRand() > MeshData->Chance) break;
            }

            // สุ่ม Static Mesh
            const int32 MeshIndex = Stream.RandRange(
                0,
                MeshData->Meshes.Num() - 1
            );

            UStaticMesh* Mesh = MeshData->Meshes[MeshIndex];

            if (!IsValid(Mesh)) break;

            
            const float ObjectScale = Stream.FRandRange(MeshData->MinScale, MeshData->MaxScale);

            
            FVector Position;
            FVector WorldPosition;

            if (!TryGetSpawnPosition(
                MeshData,
                bUseCenter,
                MinRadius,
                MaxRadius,
                Position,
                WorldPosition
            ))
            {
                continue;
            }

            
            bool bTooClose = false;
            const float MinDistance = 300.f;

            for (const FVector& ExistingPos : SpawnedPositions)
            {
                if (FVector::DistSquared(
                    WorldPosition,
                    ExistingPos
                ) < FMath::Square(MinDistance))
                {
                    bTooClose = true;
                    break;
                }
            }

            if (bTooClose) continue;

            FRotator Rotation = FRotator::ZeroRotator;

            if (bUseCenter && bFaceToCenter)
            {
                FVector Direction = CenterPosition - Position;

                Direction.Z = 0.0f;

                Rotation = Direction.Rotation();
            }
            else
            {
                Rotation.Yaw = Stream.FRandRange(0.0f, 360.0f);
            }

            UHierarchicalInstancedStaticMeshComponent* HISM = GetOrCreateHISM(Mesh);

            if (!HISM) break;

            FTransform LocalTransform;
            LocalTransform.SetLocation(Position);
            LocalTransform.SetRotation(Rotation.Quaternion());
            LocalTransform.SetScale3D(FVector(ObjectScale));

            HISM->AddInstance(LocalTransform);

            SpawnedPositions.Add(WorldPosition);

            ++CurrentSpawnCount;
        }
    };

    // =================================================
    // Spawn CenterMesh
    // =================================================

    if (StructureData->bHasCenterMesh)
    {
        const UMeshSpawnData* CenterData = StructureData->CenterMesh;

        if (!IsValid(CenterData) || CenterData->Meshes.IsEmpty()) return;

        bool bFoundCenterPosition = false;

        for (int32 Attempt = 0; Attempt < 100; ++Attempt)
        {
            if (TryGetSpawnPosition(
                CenterData,
                false,
                0.f,
                0.f,
                CenterPosition,
                CenterWorldPosition
            ))
            {
                bFoundCenterPosition = true;
                break;
            }
        }

        if (!bFoundCenterPosition) return;

        const int32 MeshIndex = Stream.RandRange(
            0,
            CenterData->Meshes.Num() - 1
        );

        UStaticMesh* Mesh = CenterData->Meshes[MeshIndex];

        if (!IsValid(Mesh)) return;

        const float ObjectScale = Stream.FRandRange(
            CenterData->MinScale,
            CenterData->MaxScale
        );

        FRotator Rotation = FRotator::ZeroRotator;
        Rotation.Yaw = Stream.FRandRange(0.f, 360.f);

        UHierarchicalInstancedStaticMeshComponent* HISM =
            GetOrCreateHISM(Mesh);

        if (!HISM) return;

        FTransform LocalTransform;
        LocalTransform.SetLocation(CenterPosition);
        LocalTransform.SetRotation(Rotation.Quaternion());
        LocalTransform.SetScale3D(FVector(ObjectScale));

        HISM->AddInstance(LocalTransform);

        bCenterSpawned = true;
    }

    // =================================================
    // Normal Spawn Meshes
    // =================================================

    if (!StructureData->bHasCenterMesh)
    {
        for (const UMeshSpawnData* MeshData : StructureData->MeshesSpawnData)
        {
            SpawnMeshData(
                MeshData,
                false,
                0.f,
                0.f,
                false
            );
        }
    }

    // =================================================
    // Spawn Meshes With CenterMesh
    // =================================================

    if (StructureData->bHasCenterMesh && bCenterSpawned)
    {
        for (const FMeshWithCenterData& Entry : StructureData->MeshesWithCenterSpawnData)
        {
            SpawnMeshData(
                Entry.MeshSpawnData,
                true,
                Entry.MinRadiusFromCenter,
                Entry.MaxRadiusFromCenter,
                Entry.FaceToCenterMesh
            );
        }
    }

    
    if (bCenterSpawned)
    {
        SpawnedPositions.Add(CenterWorldPosition);
    }
}




void ADiamondSquare::GenerateRoad()
{
	if (!OwnerChunkManager) return;
	if (!OwnerChunkManager->RoadGenerator) return;
	
	const float ChunkSize = Scale * VertexCount;
	if (!OwnerChunkManager->RoadGenerator->HasRoadInChunk(ChunkCoord)) return;
	
	TArray<FRoadData> RoadPoints = OwnerChunkManager->RoadGenerator->GetRoadDataInChunk(ChunkCoord);
	
	if (RoadPoints.Num() < 2) return;



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

		
		FVector Right = FVector::CrossProduct(FVector::UpVector, Direction);
		Right.Normalize();
		
		
		FVector LeftWorld = WorldPos - Right * (RoadWidth/2);
		FVector RightWorld = WorldPos + Right * (RoadWidth/2);


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
	
	NewHISM->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	NewHISM->SetCollisionObjectType(ECC_GameTraceChannel2);
	NewHISM->SetCollisionResponseToAllChannels(ECR_Ignore);
	NewHISM->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	HISMMap.Add(Mesh, NewHISM);

	return NewHISM;
}