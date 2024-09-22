// ChunkMeshData.h

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "ChunkMeshData.generated.h"

USTRUCT()
struct FChunkMeshSection
{
    GENERATED_BODY()

public:
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;

    int32 VertexCount = 0;

    void Clear()
    {
        Vertices.Empty();
        Triangles.Empty();
        Normals.Empty();
        UV0.Empty();
        VertexCount = 0;
    }
};


USTRUCT()
struct FChunkMeshData
{
    GENERATED_BODY()

public:
    TMap<EBlock, FChunkMeshSection> Sections;

    void Clear()
    {
        for (auto& Section : Sections)
        {
            Section.Value.Clear();
        }
        Sections.Empty();
    }
};