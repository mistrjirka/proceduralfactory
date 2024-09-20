// Fill out your copyright notice in the Description page of Project Settings.

#include "ChunkWorld.h"
#include <chrono>
#include <iostream>
#include "PerlinBasicTerrainGenerator.h"

// Sets default values
AChunkWorld::AChunkWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AChunkWorld::BeginPlay()
{
    Super::BeginPlay();

    // Ensure TerrainGenerator is set to a valid class before proceeding
    if (!TerrainGenerator)
    {
        UE_LOG(LogTemp, Error, TEXT("TerrainGenerator class is not set!"));
        return;
    }

    // Create an instance of the terrain generator using NewObject
    TerrainGeneratorInstance = NewObject<UPerlinBasicTerrainGenerator>(this, TerrainGenerator);

    // Ensure that the generator was created successfully
    if (!TerrainGeneratorInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create TerrainGenerator instance"));
        return;
    }

    // measure time taken to generate terrain
    auto start = std::chrono::high_resolution_clock::now();


    // Proceed with spawning chunks and initializing them
    for (int x = -DrawDistance; x <= DrawDistance; ++x)
    {
        for (int y = -DrawDistance; y <= DrawDistance; ++y)
        {
            ABaseChunk* SpawnedChunk = GetWorld()->SpawnActor<ABaseChunk>(Chunk, FVector(x * ChunkSize * 100, y * ChunkSize * 100, 0), FRotator::ZeroRotator);
            if (SpawnedChunk)
            {
                // Initialize the chunk with the terrain generator instance
                SpawnedChunk->Initialize(TerrainGeneratorInstance);
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    UE_LOG(LogTemp, Warning, TEXT("Time taken to generate terrain: %f"), elapsed.count());
}
