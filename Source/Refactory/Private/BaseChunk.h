// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"

#include "ChunkMeshData.h"

#include "Enums.h"
#include "Refactory/Public/FastNoiseLite.h"

#include "BaseChunk.generated.h"


enum class EDirection;
enum class EBlock;
class FastNoiseLite;
class UProceduralMeshComponent;

UCLASS(Abstract)
class REFACTORY_API ABaseChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseChunk();

	
	UPROPERTY(EditAnywhere, Category = "Chunk")
	FIntVector Size = FIntVector(1,1,1) * 32;

	UPROPERTY(EditAnywhere, Category = "Chunk")
	float Scale = 1.0;
    int VertexCount = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    TObjectPtr<UProceduralMeshComponent> Mesh;
	
    FastNoiseLite *Noise;
	
    FChunkMeshData MeshData;

	TArray<EBlock> Blocks;

	FVector GetPositionInDirection(EDirection Direction, FVector Position) const;

	int32 GetBlockIndex(int32 X, int32 Y, int32 Z) const;
    
    EBlock GetBlock(FIntVector Index) const;

    virtual void GenerateBlocks() PURE_VIRTUAL(ABaseChunk::GenerateBlocks);
    
    virtual void GenerateMesh() PURE_VIRTUAL(ABaseChunk::GenerateMesh);
    
    void ApplyMesh();
};
