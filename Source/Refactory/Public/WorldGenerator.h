#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "WorldGenerator.generated.h"

UCLASS()
class REFACTORY_API UWorldGenerator : public UObject
{
    GENERATED_BODY()
public:
    UWorldGenerator();
    ~UWorldGenerator();

    EBlock GetBlock(FIntVector Index);
};
