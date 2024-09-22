#include "Refactory/Public/BaseChunk.h"
#include "Refactory/Public/ChunkWorld.h"
#include <chrono>
#include "Kismet/GameplayStatics.h"

// Sets default values
AChunkWorld::AChunkWorld()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AChunkWorld::BeginPlay()
{
    Super::BeginPlay();
    SetActorTickEnabled(true);
    WG = NewObject<UWorldGenerator>(WorldGeneratorClass.GetDefaultObject());
    // Get all instances of generator actors
    for (auto& actorClass : generatorActors)
    {
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), actorClass, generatorActorInstances);
    }

    // Set loaded to true to enable ticking
    loaded = true;
}

void AChunkWorld::Tick(float DeltaTime)
{
    if (!loaded)
    {
        return;
    }
    Super::Tick(DeltaTime);

    // Set to keep track of all chunks that should be loaded
    TSet<FIntVector> DesiredChunks;

    // For each generator actor
    for (auto& actor : generatorActorInstances)
    {
        // Get the actor's location
        FVector actorLocation = actor->GetActorLocation();

        // Calculate the chunk coordinates for the actor
        int chunkX = FMath::FloorToInt(actorLocation.X / (ChunkSize * Scale));
        int chunkY = FMath::FloorToInt(actorLocation.Y / (ChunkSize * Scale));

        // Iterate over the chunks around the actor within the draw distance
        for (int x = -DrawDistance; x <= DrawDistance; ++x)
        {
            for (int y = -DrawDistance; y <= DrawDistance; ++y)
            {
                // Calculate the chunk coordinates
                int chunkCoordX = chunkX + x;
                int chunkCoordY = chunkY + y;

                FIntVector ChunkKey(chunkCoordX, chunkCoordY, 0);

                // Add the chunk coordinate to the desired set
                DesiredChunks.Add(ChunkKey);

                // If the chunk is already loaded, skip spawning
                if (Chunks.Contains(ChunkKey))
                {
                    continue;
                }
                UE_LOG(LogTemp, Warning, TEXT("Spawning chunk at %d, %d"), chunkCoordX, chunkCoordY);

                // Spawn a new chunk
                ABaseChunk* SpawnedChunk = GetWorld()->SpawnActor<ABaseChunk>(
                    ChunkType,
                    FVector(chunkCoordX * ChunkSize * Scale, chunkCoordY * ChunkSize * Scale, 0),
                    FRotator::ZeroRotator
                );
                if (SpawnedChunk)
                {
                    SpawnedChunk->Init(this);
                    // Add the chunk to the map
                    Chunks.Add(ChunkKey, SpawnedChunk);
                }
            }
        }
    }

    // Identify and remove chunks that are no longer needed
    TArray<FIntVector> ChunksToRemove;
    for (const auto& Pair : Chunks)
    {
        if (!DesiredChunks.Contains(Pair.Key))
        {
            ChunksToRemove.Add(Pair.Key);
        }
    }

    // Remove the unnecessary chunks
    int removed = 0;
    for (const auto& ChunkKey : ChunksToRemove)
    {
        if (ABaseChunk* ChunkToRemove = Chunks[ChunkKey])
        {
            ChunkToRemove->Destroy();
            removed++;
        }
        Chunks.Remove(ChunkKey);
    }
    if(removed)
    UE_LOG(LogTemp, Warning, TEXT("Removed %d chunks"), removed);
}
