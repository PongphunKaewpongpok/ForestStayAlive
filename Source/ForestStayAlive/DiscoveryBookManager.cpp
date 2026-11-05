// Fill out your copyright notice in the Description page of Project Settings.


#include "DiscoveryBookManager.h"

#include "DiscoveryBookEntryProgress.h"
#include "DiscoveryBookSettings.h"
#include "MySaveGame.h"
#include "DataAssets/DiscoveryBookData.h"

class UDiscoveryBookSettings;

void UDiscoveryBookManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const UDiscoveryBookSettings* Settings = GetDefault<UDiscoveryBookSettings>();
	PlantsDiscoveryDataAsset = Settings->PlantsDiscoveryDataAsset;
	AnimalsDiscoveryDataAsset = Settings->AnimalsDiscoveryDataAsset;
	
	LoadDiscoveryBook();
}

void UDiscoveryBookManager::LoadDiscoveryBook()
{
	SaveData = UMySaveGame::LoadSaveGame();
}

FDiscoveryBookEntryProgress* UDiscoveryBookManager::FindOrAddEntry(FName DiscoveryID)
{
	if (!SaveData) return nullptr;

	for (FDiscoveryBookEntryProgress& Entry : SaveData->Entries)
	{
		if (Entry.DiscoveryID == DiscoveryID) return &Entry;
	}

	FDiscoveryBookEntryProgress NewEntry;
	NewEntry.DiscoveryID = DiscoveryID;

	return &SaveData->Entries.Add_GetRef(NewEntry);
}

const FDiscoveryBookEntryProgress* UDiscoveryBookManager::FindEntry(FName DiscoveryID) const
{
	if (!SaveData) return nullptr;

	for (const FDiscoveryBookEntryProgress& Entry : SaveData->Entries)
	{
		if (Entry.DiscoveryID == DiscoveryID) return &Entry;
	}

	return nullptr;
}

void UDiscoveryBookManager::UnlockDiscoveryName(FName DiscoveryID)
{
	FDiscoveryBookEntryProgress* Entry = FindOrAddEntry(DiscoveryID);

	if (!Entry || Entry->bNameUnlocked) return;

	Entry->bNameUnlocked = true;

	SaveDiscoveryBook();
}

void UDiscoveryBookManager::UnlockDiscoveryItem(
	FName DiscoveryID,
	EDiscoveryBookSection Section,
	FName ItemID
)
{
	FDiscoveryBookEntryProgress* Entry = FindOrAddEntry(DiscoveryID);

	if (!Entry) return;

	FDiscoveryBookUnlockedItems& Items = Entry->UnlockedItems.FindOrAdd(Section);

	if (Items.ItemIDs.Contains(ItemID)) return;

	Items.ItemIDs.Add(ItemID);

	SaveDiscoveryBook();
}

bool UDiscoveryBookManager::IsDiscoveryNameUnlocked(FName DiscoveryID) const
{
	const FDiscoveryBookEntryProgress* Entry = FindEntry(DiscoveryID);

	return Entry && Entry->bNameUnlocked;
}

bool UDiscoveryBookManager::IsDiscoveryItemUnlocked(
	FName DiscoveryID,
	EDiscoveryBookSection Section,
	FName ItemID
) const
{
	const FDiscoveryBookEntryProgress* Entry = FindEntry(DiscoveryID);

	if (!Entry) return false;

	const FDiscoveryBookUnlockedItems* Items = Entry->UnlockedItems.Find(Section);

	return Items && Items->ItemIDs.Contains(ItemID);
}

void UDiscoveryBookManager::SaveDiscoveryBook()
{
	if (!IsValid(SaveData)) return;

	SaveData->SaveGame();
}

UDiscoveryBookData* UDiscoveryBookManager::GetDiscoveryDataByIndex(FName Category, int32 Index) const
{
	if (Category == FName("Animals"))
	{
		if (!AnimalsDiscoveryDataAsset.IsValidIndex(Index)) return nullptr;

		return AnimalsDiscoveryDataAsset[Index].LoadSynchronous();
	}

	if (Category == FName("Plants"))
	{
		if (!PlantsDiscoveryDataAsset.IsValidIndex(Index)) return nullptr;

		return PlantsDiscoveryDataAsset[Index].LoadSynchronous();
	}

	return nullptr;
}

int32 UDiscoveryBookManager::GetDiscoveryDataSize(const FName Category) const
{
	if (Category == FName("Animals")) return AnimalsDiscoveryDataAsset.Num();
	if (Category == FName("Plants")) return PlantsDiscoveryDataAsset.Num();

	return 0;
}