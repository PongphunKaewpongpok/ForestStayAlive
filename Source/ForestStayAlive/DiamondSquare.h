// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProceduralMeshComponent.h"
#include "SpawnObjectData.h"

#include "DiamondSquare.generated.h"


class UProceduralMeshComponent;
class UMaterialInterface;
class UHierarchicalInstancedStaticMeshComponent;
class AChunkManager;

UCLASS()
class FORESTSTAYALIVE_API ADiamondSquare : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADiamondSquare();
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true", ClampMin = "0"))
	int VertexCount;
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true", ClampMin = "0"))
	float ZMultiplier;
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true", ClampMin = "0"))
	float NoiseScale;
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true", ClampMin = "0.000001"))
	float Scale;
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true", ClampMin = "0.000001"))
	float UVScale;
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	UMaterialInterface* Material;
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true", ClampMin="-999999", ClampMax="999999"))
	int32 Seed;
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	FIntPoint ChunkCoord;
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	bool bHasRiver = false;
	UPROPERTY(BlueprintReadWrite, Meta = (ClampMin="0"))
	int32 RiverRegionSize;
	UPROPERTY(BlueprintReadWrite, Meta = (ClampMin="0", ClampMax="1"))
	float RiverChance;
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	float RiverScale;
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	float RiverWidth;
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	float RiverDepth;
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	float WaterLevel;
	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	bool bHasRoad = false;
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	float RoadWidth;
	
	
	
	UPROPERTY()
	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> HISMMap;

	UHierarchicalInstancedStaticMeshComponent* GetOrCreateHISM(UStaticMesh* Mesh);
	
	UFUNCTION()
	int32 GetChunkSeed(const int32 Salt) const;
	UFUNCTION(BlueprintCallable)
	void GenerateTerrain();
	UFUNCTION(BlueprintCallable)
	void GenerateRiver();
	UFUNCTION(BlueprintCallable)
	void GenerateWaterTerrain();
	
	
	
	UFUNCTION(BlueprintCallable)
	void GenerateStructures();
	UFUNCTION(BlueprintCallable)
	void GenerateObject(const TArray<FSpawnObjectData>& SpawnObjects, const int SpawnCount);
	

	
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	UMaterialInterface* WaterMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	AChunkManager* OwnerChunkManager;

private:
	UPROPERTY()
	UProceduralMeshComponent* ProceduralMesh;
	
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<struct FProcMeshTangent> Tangents;
	
	TArray<FVector> WaterVertices;
	TArray<int32> WaterTriangles;
	TArray<FVector2D> WaterUV;
	TArray<FVector> WaterNormals;
	TArray<FProcMeshTangent> WaterTangents;
	
	TArray<bool> RiverMask;
	
	void CreateVertices();
	void CreateTriangles();
	
	void GenerateRoad();
	float GetTerrainHeight(float WorldX, float WorldY);
	
	TArray<float> HeightMap;
};
