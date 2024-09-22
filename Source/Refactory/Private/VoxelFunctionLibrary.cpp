// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelFunctionLibrary.h"

FIntVector UVoxelFunctionLibrary::WorldToBlockPosition(const FVector &Position)
{
    return FIntVector(Position) / Scale;
}

FIntVector UVoxelFunctionLibrary::WorldToLocalBlockPosition(const FVector &Position, const int32 Size)
{
    const auto ChunkPos = WorldToChunkPosition(Position, Size);

    auto Result = WorldToBlockPosition(Position) - ChunkPos * Size;

    // Negative Normalization

    Result.X-= ChunkPos.X < 0;
    Result.Y-= ChunkPos.Y < 0;
    Result.Z-= ChunkPos.Z < 0;

    return Result;
}

FIntVector UVoxelFunctionLibrary::WorldToChunkPosition(const FVector &Position, const int32 Size)
{
    FIntVector Result;

    const int Factor = Size * Scale;
    const auto IntPosition = FIntVector(Position);
    Result.X = static_cast<int>(Position.X / Factor) - static_cast<int>(IntPosition.X < 0);
    Result.Y = static_cast<int>(Position.Y / Factor) - static_cast<int>(IntPosition.Y < 0);
    Result.Z = static_cast<int>(Position.Z / Factor) - static_cast<int>(IntPosition.Z < 0);

    return Result;
}
