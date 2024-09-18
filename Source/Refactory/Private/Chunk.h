// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"


#include "Enums.h"
#include "Refactory/Public/FastNoiseLite.h"

#include "Chunk.generated.h"


enum class EDirection;
enum class EBlock;
class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS()
class REFACTORY_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

	UPROPERTY(EditAnywhere, Category = "Chunk")
	int32 Size = 32;

	UPROPERTY(EditAnywhere, Category = "Chunk")
	int32 Scale = 1;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TObjectPtr<UProceduralMeshComponent> Mesh;
	FastNoiseLite *Noise;

	TArray<EBlock> Blocks;
	TArray<FVector> VertexData;
	TArray<int32> TriangleData;
	TArray<FVector2D> UVData;

	int32 VertexCount = 0;
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

	void GenerateBlocks();

	void GenerateMesh();

	void ApplyMesh() const;

	bool Check(FVector Position) const;

	void CreateFace(EDirection Direction, FVector Position);

	TArray<FVector> GetFaceVertices(EDirection Direction, FVector Position) const;

	FVector GetPositionInDirection(EDirection Direction, FVector Position) const;

	int32 GetBlockIndex(int32 X, int32 Y, int32 Z) const;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
