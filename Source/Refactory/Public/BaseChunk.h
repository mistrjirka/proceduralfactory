// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Refactory/Public/ChunkMeshData.h"
#include "Refactory/Public/ChunkWorld.h"

#include "Enums.h"

#include "BaseChunk.generated.h"


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
    float Scale = 100.0;

    UPROPERTY(EditAnywhere, Category = "Materials")
    TMap<EBlock, UMaterialInterface*> BlockTypeMaterials;

    UMaterialInterface* GetMaterialForBlockType(EBlock BlockType) const;

    int VertexCount = 0;

    UFUNCTION(BlueprintCallable, Category = "Chunk")
    void ModifyVoxel(const FIntVector Position, EBlock ModifyTo);

    void Init(AChunkWorld* W);


protected:
    UPROPERTY()
    UMaterialInterface* GrassMaterial;

    UPROPERTY()
    UMaterialInterface* DirtMaterial;

    UPROPERTY()
    UMaterialInterface* StoneMaterial;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    TWeakObjectPtr<AChunkWorld> World;

    TObjectPtr<UProceduralMeshComponent> Mesh;

    FChunkMeshData MeshData;

    TArray<EBlock> Blocks;

    FVector GetPositionInDirection(EDirection Direction, FVector Position) const;

    int32 GetBlockIndex(int32 X, int32 Y, int32 Z) const;
    
    EBlock GetBlock(FIntVector Index) const;

    virtual void GenerateMesh() PURE_VIRTUAL(ABaseChunk::GenerateMesh);

    virtual void ModifyVoxelData(const FIntVector Position, EBlock ModifyTo) PURE_VIRTUAL(ABaseChunk::ModifyVoxelData);

    void LoadChunkData();

    void ClearMesh();
    
    void ApplyMesh();
};
