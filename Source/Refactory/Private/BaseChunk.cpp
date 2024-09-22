#include "Refactory/Public/BaseChunk.h"
#include "BaseChunk.h"

ABaseChunk::ABaseChunk()
{
    PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
    Blocks.Init(EBlock::Air, Size.X * Size.Y * Size.Z);
    Mesh->SetCastShadow(false);
    SetRootComponent(Mesh);
}



int ABaseChunk::GetBlockIndex(int X, int Y, int Z) const
{
    return Z * Size.X * Size.Y + Y * Size.X + X;
}

void ABaseChunk::BeginPlay()
{
    Super::BeginPlay();
    LoadChunkData();
}

EBlock ABaseChunk::GetBlock(FIntVector Index) const
{
    if (Index.X < 0 || Index.X >= Size.X || Index.Y < 0 || Index.Y >= Size.Y || Index.Z < 0 || Index.Z >= Size.Z)
    {
        return EBlock::Air;
    }
    return Blocks[GetBlockIndex(Index.X, Index.Y, Index.Z)];
}

FVector ABaseChunk::GetPositionInDirection(EDirection Direction, FVector Position) const
{
    switch (Direction)
    {
    case EDirection::Forward:
        return Position + FVector::ForwardVector;
        break;
    case EDirection::Right:
        return Position + FVector::RightVector;
        break;
    case EDirection::Back:
        return Position + FVector::BackwardVector;
        break;
    case EDirection::Left:
        return Position + FVector::LeftVector;
        break;
    case EDirection::Up:
        return Position + FVector::UpVector;
        break;
    case EDirection::Down:
        return Position + FVector::DownVector;
        break;
    default:
        return FVector(0, 0, 0);
    }
}

void ABaseChunk::ApplyMesh()
{
    Mesh->CreateMeshSection(0, MeshData.Vertices, MeshData.Triangles, MeshData.Normals, MeshData.UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

void ABaseChunk::LoadChunkData()
{

    FVector Location = GetActorLocation();

    for (int x = 0; x < Size.X; x++)
    {
        for (int y = 0; y < Size.Y; y++)
        {
            const float Xpos = (x * 100 + Location.X) / 100;
            const float Ypos = (y * 100 + Location.Y) / 100;

            const int Height = 10; // FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(Xpos, Ypos) + 1) * Size.Z / 2), 0, Size.Z);

            for (int z = 0; z < Height; z++)
            {
                Blocks[GetBlockIndex(x, y, z)] = EBlock::Stone;
            }

            for (int z = Height; z < Size.Z; z++)
            {
                Blocks[GetBlockIndex(x, y, z)] = EBlock::Air;
            }
        }
    }

    GenerateMesh();
    ApplyMesh();
    UE_LOG(LogTemp, Warning, TEXT("Vertex Count: %d"), VertexCount);
}

void ABaseChunk::ClearMesh()
{
    MeshData.Clear();
    VertexCount = 0;
}

void ABaseChunk::ModifyVoxel(const FIntVector Position, EBlock ModifyTo)
{
    if (Position.X < 0 || Position.X >= Size.X || Position.Y < 0 || Position.Y >= Size.Y || Position.Z < 0 || Position.Z >= Size.Z)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Position"));
        return;
    }

    /*UE_LOG(LogTemp, Warning, TEXT("Modifying Voxel at Position: %s to %d"), *Position.ToString(), static_cast<uint8>(ModifyTo));*/

    ModifyVoxelData(Position, ModifyTo);

    ClearMesh();

    GenerateMesh();

    ApplyMesh();
}