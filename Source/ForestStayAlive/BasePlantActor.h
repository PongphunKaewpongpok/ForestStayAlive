// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ForestStayAlive/Interfaces/Photographable.h"
#include "GameFramework/Actor.h"
#include "BasePlantActor.generated.h"

class UHighlightableObject;

UCLASS()
class FORESTSTAYALIVE_API ABasePlantActor : public AActor, public IPhotographable
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UDiscoveryBookData* DiscoveryData;
	
public:	
	// Sets default values for this actor's properties
	ABasePlantActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	TSubclassOf<UHighlightableObject> HighlightableObjectClass;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual UDiscoveryBookData* GetDiscoveryBookData_Implementation() const override;
	virtual bool AddNewDataToDiscoveryBook_Implementation() const override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials")
	TObjectPtr<UHighlightableObject> HighlightableObject;
	
};
