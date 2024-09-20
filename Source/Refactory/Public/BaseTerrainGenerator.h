// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"

#include "BaseTerrainGenerator.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class REFACTORY_API UBaseTerrainGenerator : public UObject
{
	GENERATED_BODY()
public:
	virtual void GenerateTerrain(TArray<EBlock>& Blocks, const FVector& Location, const FIntVector& Size, float Scale) PURE_VIRTUAL(BaseTerrainGenerator::GenerateTerrain);

	int GetBlockIndex(int X, int Y, int Z, const FIntVector &Size) const
	{
		return Z * Size.X * Size.Y + Y * Size.X + X;
	}

};
