// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseChunk.h"
#include "ChunkWorld.generated.h"

UCLASS()
class AChunkWorld : public AActor
{
    GENERATED_BODY()

public:    
    // Sets default values for this actor's properties
    AChunkWorld();

    UPROPERTY(EditAnywhere, Category = "Chunk World")
    TSubclassOf<ABaseChunk> Chunk;

    UPROPERTY(EditAnywhere, Category = "Chunk World")
    TSubclassOf<UBaseTerrainGenerator> TerrainGenerator;
    
    UPROPERTY(EditAnywhere, Category = "Chunk World")
    TArray<TObjectPtr<AActor>> generatorActors;

    UPROPERTY(EditAnywhere, Category = "Chunk World")
    int DrawDistance = 5;

    UPROPERTY(EditAnywhere, Category = "Chunk World")
    int ChunkSize = 32;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Use raw pointer or TWeakObjectPtr for TerrainGenerator
    UBaseTerrainGenerator* TerrainGeneratorInstance;
};
