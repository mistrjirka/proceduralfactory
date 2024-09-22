// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldGenerator.h"

#include "ChunkWorld.generated.h"

class ABaseChunk;

UCLASS()
class REFACTORY_API AChunkWorld : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AChunkWorld();

    UPROPERTY(EditAnywhere, Category = "Chunk World")
    TSubclassOf<ABaseChunk> ChunkType;

    UPROPERTY(EditAnywhere, Category = "Chunk World")
    TArray<TSubclassOf<AActor>> generatorActors;

    UPROPERTY(EditAnywhere, Category = "Chunk World")
    int DrawDistance = 5;

    UPROPERTY(EditAnywhere, Category = "Chunk World")
    int ChunkSize = 32;

    UPROPERTY(EditAnywhere, Category = "Chunk World")
    TSubclassOf<UWorldGenerator> WorldGeneratorClass;


    // Called every frame
    virtual void Tick(float DeltaTime) override;

    TObjectPtr<UWorldGenerator> WG;
protected:
    float Scale = 100.0;

    bool loaded = false;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void GenerateTerrainAroundActor(AActor* actor);

    TMap<FIntVector, ABaseChunk*> Chunks;

    TArray<AActor*> generatorActorInstances;

};
