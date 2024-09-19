// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"

#include "BaseChunk.h"
#include "Enums.h"
#include "Refactory/Public/FastNoiseLite.h"

#include "Chunk.generated.h"


class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS()
class REFACTORY_API AChunk : public ABaseChunk
{
	GENERATED_BODY()

private:
	//points on a cube
	const FVector BlockVectorData[8] = {
		FVector(100, 100, 100),
		FVector(100, 0, 100),
		FVector(100, 0, 0),
		FVector(100, 100, 0),
		FVector(0,0,100),
		FVector(0,100,100),
		FVector(0,100,0),
		FVector(0,0,0)
	};

	const int32 BlockTriangleData[24] = 
	{
		0,1,2,3,
		5,0,3,6,
		4,5,6,7,
		1,4,7,2,
		5,4,1,0,
		3,2,7,6
	};

	bool Check(FVector Position) const;

	void CreateFace(EDirection Direction, FVector Position);

	TArray<FVector> GetFaceVertices(EDirection Direction, FVector Position) const;

protected:
	virtual void GenerateMesh() override;
	virtual void GenerateBlocks() override;
	virtual void ModifyVoxelData(const FIntVector Position, EBlock ModifyTo) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
