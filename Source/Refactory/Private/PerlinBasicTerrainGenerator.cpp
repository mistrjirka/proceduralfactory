// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinBasicTerrainGenerator.h"

UPerlinBasicTerrainGenerator::UPerlinBasicTerrainGenerator()
{
    Noise = new FastNoiseLite();
    Noise->SetFrequency(0.03f);
    Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    Noise->SetFractalType(FastNoiseLite::FractalType_FBm);
}

void UPerlinBasicTerrainGenerator::GenerateTerrain(TArray<EBlock> &Blocks, const FVector &Location, const FIntVector &Size, float Scale)
{
    for (int x = 0; x < Size.X; x++)
    {
        for (int y = 0; y < Size.Y; y++)
        {
            const float Xpos = (x * 100 + Location.X) / 100;
            const float Ypos = (y * 100 + Location.Y) / 100;

            const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(Xpos, Ypos) + 1) * Size.Z / 2), 0, Size.Z);

            for (int z = 0; z < Height; z++)
            {
                Blocks[GetBlockIndex(x, y, z, Size)] = EBlock::Stone;
            }

            for (int z = Height; z < Size.Z; z++)
            {
                Blocks[GetBlockIndex(x, y, z, Size)] = EBlock::Air;
            }
        }
    }
}
