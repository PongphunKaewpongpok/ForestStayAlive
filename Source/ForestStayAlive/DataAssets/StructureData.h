// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StructureData.generated.h"

class UMeshSpawnData;



USTRUCT(BlueprintType)
struct FMeshWithCenterData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMeshSpawnData> MeshSpawnData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinRadiusFromCenter = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRadiusFromCenter = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FaceToCenterMesh = true;
};



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
	bool bHasCenterMesh = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bHasCenterMesh", EditConditionHides), Category = "Mesh")
	TObjectPtr<UMeshSpawnData> CenterMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bHasCenterMesh", EditConditionHides), Category = "Mesh")
	TArray<FMeshWithCenterData> MeshesWithCenterSpawnData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bHasCenterMesh", EditConditionHides), Category = "Mesh")
	TArray<TObjectPtr<UMeshSpawnData>> MeshesSpawnData;
};

