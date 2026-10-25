// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StructureData.generated.h"

class UMeshSpawnData;
/**
 * 
 */
UCLASS(BlueprintType)
class FORESTSTAYALIVE_API UStructureData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Type = TEXT("None");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Chance = 0.01f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<TObjectPtr<UMeshSpawnData>> MeshSpawnDatas;
};