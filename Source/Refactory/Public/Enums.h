#pragma once

UENUM(BlueprintType)
enum class EDirection : uint8
{
    Forward, Right, Back, Left, Up, Down
};


UENUM(BlueprintType)
enum class EBlock : uint8
{
    Null, Air, Stone, Dirt, Grass
};