// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkManager.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "RoadGenerator.h"

// Sets default values
AChunkManager::AChunkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChunkManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (bHasRoad)
	{
		RoadGenerator = NewObject<URoadGenerator>();
		RoadGenerator->Initialize(Seed, RoadWidth, RenderDistance, RoadPointsPerChunk, VertexCount * Scale);
		RoadGenerator->GenerateRoad();
	}
		
	GenerateChunkAroundPlayer();
}

// Called every frame
void AChunkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RoadGenerator->UpdateAutoGenerateRoadPoints(GetPlayerChunk());
	UpdateChunks();
}



void AChunkManager::RandomSeed()
{
	Seed = FMath::RandRange(-999999, 999999);
}

void AChunkManager::SetSeed(const int NewSeed)
{
	Seed = NewSeed;
}




FIntPoint AChunkManager::GetPlayerChunk() const
{
	ACharacter* PlayerCharacter =
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		FVector Location = PlayerCharacter->GetActorLocation();
		
		return FIntPoint(FMath::FloorToInt(Location.X/(VertexCount*Scale)), FMath::FloorToInt(Location.Y/(VertexCount*Scale)));
	}
	return FIntPoint();
}

void AChunkManager::SpawnChunk(const FIntPoint ChunkCoord)
{
	if (LoadedChunks.Contains(ChunkCoord))
	{
		return;
	}

	FVector Location(
		(ChunkCoord.X * VertexCount * Scale),
		(ChunkCoord.Y * VertexCount * Scale),
		0.0f);

	ADiamondSquare* Chunk =
		GetWorld()->SpawnActor<ADiamondSquare>(
			ADiamondSquare::StaticClass(),
			Location,
			FRotator::ZeroRotator);

	if (Chunk)
	{
		Chunk->OwnerChunkManager = this;
		
		Chunk->VertexCount = VertexCount;
		Chunk->ZMultiplier = ZMultiplier;
		Chunk->NoiseScale = NoiseScale;
		Chunk->Scale = Scale;
		Chunk->UVScale = UVScale;
		Chunk->Seed = Seed;
		Chunk->ChunkCoord = ChunkCoord;
		Chunk->Material = Material;
		
		Chunk->bHasRiver = bHasRiver;
		Chunk->RiverRegionSize = RiverRegionSize;
		Chunk->RiverChance = RiverChance;
		Chunk->RiverScale = RiverScale;
		Chunk->RiverWidth = RiverWidth;
		Chunk->RiverDepth = RiverDepth;
		
		Chunk->WaterLevel = WaterLevel;
		Chunk->WaterMaterial = WaterMaterial;
		
		Chunk->bHasRoad = bHasRoad;
		Chunk->RoadWidth = RoadWidth;

		Chunk->GenerateTerrain();
		
		// Generate General Structure (Abandoned Campsite and more..)
		Chunk->GenerateStructures(StructuresList);
		
		// Generate General Object (Trees, Rocks and more..)
		Chunk->GenerateObject(Structure_Forest);

		LoadedChunks.Add(ChunkCoord, Chunk);
	}
}

void AChunkManager::GenerateChunkAroundPlayer()
{
	FIntPoint PlayerChunk = GetPlayerChunk();
	
	for (int x = -RenderDistance; x <= RenderDistance; ++x)
	{
		for (int y = -RenderDistance; y <= RenderDistance; ++y)
		{
			SpawnChunk(FIntPoint(PlayerChunk.X + x, PlayerChunk.Y + y));
		}
	}
}

void AChunkManager::UnloadUnusedChunks()
{
	FIntPoint PlayerChunk = GetPlayerChunk();
	TSet<FIntPoint> ChunksToUnload;
	
	for (const TPair<FIntPoint, ADiamondSquare*>& Pair : LoadedChunks)
	{
		if (!((Pair.Key.X >= (PlayerChunk.X-RenderDistance) && Pair.Key.X <= (PlayerChunk.X+RenderDistance))
			&& (Pair.Key.Y >= (PlayerChunk.Y-RenderDistance) && Pair.Key.Y <= (PlayerChunk.Y+RenderDistance))))
		{
			ChunksToUnload.Add(Pair.Key);
		}
	}
	
	for (const FIntPoint& Coord : ChunksToUnload)
	{
		if (ADiamondSquare** Chunk = LoadedChunks.Find(Coord))
		{
			(*Chunk)->Destroy();
		}

		LoadedChunks.Remove(Coord);
	}
}

void AChunkManager::UpdateChunks()
{
	if (LatestPlayerChunk != GetPlayerChunk())
	{
		LatestPlayerChunk = GetPlayerChunk();
		
		GenerateChunkAroundPlayer();
		UnloadUnusedChunks();
	}
}