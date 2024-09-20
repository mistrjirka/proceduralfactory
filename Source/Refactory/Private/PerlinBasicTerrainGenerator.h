// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTerrainGenerator.h"
#include "Refactory/Public/FastNoiseLite.h"

#include "PerlinBasicTerrainGenerator.generated.h"

/**
 * 
 */
UCLASS()
class UPerlinBasicTerrainGenerator : public UBaseTerrainGenerator
{
	GENERATED_BODY()

public:
	UPerlinBasicTerrainGenerator();

	virtual void GenerateTerrain(TArray<EBlock>& Blocks, const FVector& Location, const FIntVector& Size, float Scale) override;

private:
    FastNoiseLite *Noise;
};
