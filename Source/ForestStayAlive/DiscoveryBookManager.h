// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DiscoveryBookManager.generated.h"

class UDiscoveryBookData;
struct FDiscoveryBookEntryProgress;
/**
 * 
 */
class UMySaveGame;

UCLASS()
class FORESTSTAYALIVE_API UDiscoveryBookManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Discovery Book")
	void UnlockDiscoveryName(FName DiscoveryID);

	UFUNCTION(BlueprintCallable, Category = "Discovery Book")
	void UnlockDiscoveryItem(
		FName DiscoveryID,
		EDiscoveryBookSection Section,
		FName ItemID
	);

	UFUNCTION(BlueprintPure, Category = "Discovery Book")
	bool IsDiscoveryNameUnlocked(FName DiscoveryID) const;

	UFUNCTION(BlueprintPure, Category = "Discovery Book")
	bool IsDiscoveryItemUnlocked(
		FName DiscoveryID,
		EDiscoveryBookSection Section,
		FName ItemID
	) const;

	UFUNCTION(BlueprintCallable, Category = "Discovery Book")
	void SaveDiscoveryBook();
	
	UFUNCTION(BlueprintCallable, Category = "Discovery Book")
	UDiscoveryBookData* GetDiscoveryDataByIndex(FName Category, int32 Index) const;
	
	UFUNCTION(BlueprintCallable, Category = "Discovery Book")
	int32 GetDiscoveryDataSize(const FName Category) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Discovery Book")
	TArray<TSoftObjectPtr<UDiscoveryBookData>> PlantsDiscoveryDataAsset;
	UPROPERTY(EditDefaultsOnly, Category = "Discovery Book")
	TArray<TSoftObjectPtr<UDiscoveryBookData>> AnimalsDiscoveryDataAsset;

	UPROPERTY()
	TObjectPtr<UMySaveGame> SaveData;

	void LoadDiscoveryBook();

	FDiscoveryBookEntryProgress* FindOrAddEntry(FName DiscoveryID);
	const FDiscoveryBookEntryProgress* FindEntry(FName DiscoveryID) const;
};