#include "SpawnObjectData.h"

#include "UObject/UObjectGlobals.h"


// Utils
float FSpawnObjectLibrary::GetTotalChance(const TArray<FSpawnObjectData>& SpawnObjects)
{
	float TotalChance = 0.0f;

	for (const FSpawnObjectData& Data : SpawnObjects)
	{
		if (Data.Chance <= 0.f) { continue; }

		TotalChance += Data.Chance;
	}

	return TotalChance;
}




// Objects Data

FSpawnObjectData FSpawnObjectLibrary::Tree()
{
	FSpawnObjectData Data;
	
	Data.Meshes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/HighPoly_Tree_Model/Meshes/SM_HP_Tree.SM_HP_Tree")));
	
	Data.Count = 1;
	Data.Chance = 1.5f;
	Data.bAlignToSurface = true;

	Data.MinScale = 0.8f;
	Data.MaxScale = 1.3f;

	Data.MinHeight = -550.0f;
	Data.MaxHeight = 100000.0f;

	Data.MinSlope = 0.0f;
	Data.MaxSlope = 10.0f;

	return Data;
}

FSpawnObjectData FSpawnObjectLibrary::Rock()
{
	FSpawnObjectData Data;

	Data.Meshes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/Rocks/Granite_Rock_ohFsZ/Granite_Rock_ohFsZ_Mid.Granite_Rock_ohFsZ_Mid")));
	Data.Meshes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/Rocks/Granite_Rock_pkeeM/Granite_Rock_pkeeM_Mid.Granite_Rock_pkeeM_Mid")));
	Data.Meshes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/Rocks/Mossy_Forest_Rock_vimrfjsaw/Mossy_Forest_Rock_vimrfjsaw_Mid.Mossy_Forest_Rock_vimrfjsaw_Mid")));
	Data.Meshes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/Rocks/Nordic_Forest_Rock_Small_xjijbgx/Nordic_Forest_Rock_Small_xjijbgx_Mid.Nordic_Forest_Rock_Small_xjijbgx_Mid")));
	
	Data.Count = 1;
	Data.Chance = 0.05f;
	Data.bAlignToSurface = true;

	Data.MinScale = 4.0f;
	Data.MaxScale = 5.0f;

	Data.MinHeight = -100000.f;
	Data.MaxHeight = 100000.f;

	Data.MinSlope = 0.f;
	Data.MaxSlope = 45.f;

	return Data;
}

FSpawnObjectData FSpawnObjectLibrary::Bush()
{
	FSpawnObjectData Data;

	Data.Meshes.Add(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Assets/HighPoly_Tree_Model/Meshes/SM_HP_Tree.SM_HP_Tree")));
	
	Data.Count = 1;
	Data.Chance = 0.7f;
	Data.bAlignToSurface = true;

	Data.MinScale = 0.6f;
	Data.MaxScale = 1.1f;

	Data.MinHeight = 50.f;
	Data.MaxHeight = 5000.f;

	Data.MinSlope = 0.f;
	Data.MaxSlope = 20.f;

	return Data;
}




// GetSpawnObjects
TArray<FSpawnObjectData> FSpawnObjectLibrary::GetSpawnObjects_Forest()
{
	return {
		Rock(),
		Tree()
	};
}