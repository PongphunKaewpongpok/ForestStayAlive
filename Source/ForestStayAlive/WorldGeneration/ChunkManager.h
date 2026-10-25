// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DiamondSquare.h"

#include "ChunkManager.generated.h"

class ADiamondSquare;
class URoadGenerator;

UCLASS()
class FORESTSTAYALIVE_API AChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkManager();
	
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "0"))
	int VertexCount;
	
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "0"))
	float ZMultiplier;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "0"))
	float NoiseScale;
	
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "0.000001"))
	float Scale;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "0.000001"))
	float UVScale;
	
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;
	
	UPROPERTY(EditAnywhere, Meta = (ClampMin="-999999", ClampMax="999999"))
	int32 Seed;
	
	
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "1"))
	int8 RenderDistance;
	
	
	UPROPERTY(EditAnywhere)
	bool bHasRiver = false;
	UPROPERTY(EditAnywhere, Meta = (EditCondition="bHasRiver", EditConditionHides, ClampMin="0"))
	int32 RiverRegionSize;
	UPROPERTY(EditAnywhere, Meta = (EditCondition="bHasRiver", EditConditionHides, ClampMin="0", ClampMax="1"))
	float RiverChance;
	UPROPERTY(EditAnywhere, Meta = (EditCondition="bHasRiver", EditConditionHides))
	float RiverScale;
	UPROPERTY(EditAnywhere, Meta = (EditCondition="bHasRiver", EditConditionHides))
	float RiverWidth;
	UPROPERTY(EditAnywhere, Meta = (EditCondition="bHasRiver", EditConditionHides))
	float RiverDepth;
	
	UPROPERTY(EditAnywhere, Meta = (EditCondition="bHasRiver", EditConditionHides))
	float WaterLevel;
	UPROPERTY(EditAnywhere, Meta = (EditCondition="bHasRiver", EditConditionHides))
	UMaterialInterface* WaterMaterial;
	
	UPROPERTY(Category="Road", EditAnywhere)
	bool bHasRoad = false;
	UPROPERTY(Category="Road", EditAnywhere, Meta = (EditCondition="bHasRoad", EditConditionHides, ClampMin="0"))
	float RoadWidth;
	UPROPERTY(Category="Road", EditAnywhere, Meta = (EditCondition="bHasRoad", EditConditionHides, ClampMin="2"))
	int8 RoadPointsPerChunk = 2;
	
	UPROPERTY(Category="Structures", EditDefaultsOnly)
	TObjectPtr<UStructureData> Structure_Forest;
	UPROPERTY(Category="Structures", EditDefaultsOnly)
	TArray<TObjectPtr<UStructureData>> StructuresList;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	URoadGenerator* RoadGenerator;
	
	
private:
	UPROPERTY()
	TMap<FIntPoint, ADiamondSquare*> LoadedChunks;
	UPROPERTY()
	FIntPoint LatestPlayerChunk;
	
	FIntPoint GetPlayerChunk() const;
	
	UFUNCTION(BlueprintCallable)
	void RandomSeed();
	UFUNCTION(BlueprintCallable)
	void SetSeed(int NewSeed);
	
	void SpawnChunk(const FIntPoint ChunkCoord);
	void GenerateChunkAroundPlayer();
	void UnloadUnusedChunks();
	void UpdateChunks();
};
