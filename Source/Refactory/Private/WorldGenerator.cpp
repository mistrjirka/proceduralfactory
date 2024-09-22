#include "Refactory/Public/WorldGenerator.h"

UWorldGenerator::UWorldGenerator()
{
}

UWorldGenerator::~UWorldGenerator()
{
}

EBlock UWorldGenerator::GetBlock(FIntVector Index)
{
    if (Index.Z < 10)
    {
        return EBlock::Stone;
    }
    else
    {
        return EBlock::Air;
    }
}

