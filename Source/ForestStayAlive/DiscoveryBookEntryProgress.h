#pragma once

#include "CoreMinimal.h"
#include "DiscoveryBookEntryProgress.generated.h"

UENUM(BlueprintType)
enum class EDiscoveryBookSection : uint8
{
	Appearances,
	Benefits,
	Precautions,
	FoundInLocations
};

USTRUCT(BlueprintType)
struct FDiscoveryBookUnlockedItems
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<FName> ItemIDs;
};

USTRUCT(BlueprintType)
struct FDiscoveryBookEntryProgress
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FName DiscoveryID;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bNameUnlocked = false;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TMap<EDiscoveryBookSection, FDiscoveryBookUnlockedItems> UnlockedItems;
};