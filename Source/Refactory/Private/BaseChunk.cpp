#include "BaseChunk.h"

ABaseChunk::ABaseChunk()
{
    PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Noise = new FastNoiseLite();
	Noise->SetFrequency(0.03f);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	Noise->SetFractalType(FastNoiseLite::FractalType_FBm);

	Blocks.SetNum(Size.X * Size.Y * Size.Z);

	Mesh->SetCastShadow(true);
	SetRootComponent(Mesh);
}

int ABaseChunk::GetBlockIndex(int X, int Y, int Z) const
{
	return Z * Size.X * Size.Y + Y * Size.X + X;
}

void ABaseChunk::BeginPlay()
{
    Super::BeginPlay();
    GenerateBlocks();
    GenerateMesh();
    ApplyMesh();

    UE_LOG(LogTemp, Warning, TEXT("Vertex Count: %d"), VertexCount);
}

EBlock ABaseChunk::GetBlock(FIntVector Index) const
{
	if(Index.X < 0 || Index.X >= Size.X || Index.Y < 0 || Index.Y >= Size.Y || Index.Z < 0 || Index.Z >= Size.Z)
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

void ABaseChunk::ClearMesh()
{
    MeshData.Clear();
    VertexCount = 0;
}

void ABaseChunk::ModifyVoxel(const FIntVector Position, EBlock ModifyTo)
{
    if(Position.X < 0 || Position.X >= Size.X || Position.Y < 0 || Position.Y >= Size.Y || Position.Z < 0 || Position.Z >= Size.Z)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Position"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Modifying Voxel at Position: %s to %d"), *Position.ToString(), static_cast<uint8>(ModifyTo));

    ModifyVoxelData(Position, ModifyTo);
    
    ClearMesh();

    GenerateMesh();
    
    ApplyMesh();
}