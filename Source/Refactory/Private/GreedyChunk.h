// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "BaseChunk.h"

#include "Enums.h"
#include "Refactory/Public/FastNoiseLite.h"
#include "GreedyChunk.generated.h"

class UProceduralMeshComponent;

UCLASS()
class REFACTORY_API AGreedyChunk : public ABaseChunk
{
	GENERATED_BODY()

	struct FMask
	{
		EBlock Block;
		int32 Normal;
	};

private:
	void CreateQuad(FMask Mask, FIntVector AxisMask, FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4);

	bool CompareMask(FMask M1, FMask M2) const;
protected:

	virtual void GenerateMesh() override;
	virtual void ModifyVoxelData(const FIntVector Position, EBlock ModifyTo) override;

};
