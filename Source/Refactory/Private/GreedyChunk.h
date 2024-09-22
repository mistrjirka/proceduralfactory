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
	void CreateQuad(FMask Mask, FIntVector AxisMask, const int Width, const int Height, FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4);
    FVector2D GetUVOffset(EBlock BlockType, const FVector& Normal);
	bool CompareMask(FMask M1, FMask M2) const;
protected:
    void AdjustUVsForOrientation(
		FVector2D& UV1,
		FVector2D& UV2,
		FVector2D& UV3,
		FVector2D& UV4,
		const FIntVector& AxisMask,
		int32 NormalDirection
	);
	virtual void GenerateMesh() override;
	virtual void ModifyVoxelData(const FIntVector Position, EBlock ModifyTo) override;

};
