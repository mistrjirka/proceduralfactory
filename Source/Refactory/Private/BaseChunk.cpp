#include "Refactory/Public/ChunkWorld.h"
#include "Refactory/Public/BaseChunk.h"

ABaseChunk::ABaseChunk()
{
    PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
    Mesh->SetCastShadow(false);
    SetRootComponent(Mesh);
    this->Scale = 100.0f;
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> GrassMat(TEXT("/Game/StarterContent/Materials/M_Ground_Moss.M_Ground_Moss"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> DirtMat(TEXT("/Game/StarterContent/Materials/M_Brick_Cut_Stone.M_Brick_Cut_Stone"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> StoneMat(TEXT("/Game/StarterContent/Materials/M_Concrete_Grime.M_Concrete_Grime"));

    if (GrassMat.Succeeded())
    {
        GrassMaterial = GrassMat.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load GrassMaterial"));
    }

    if (DirtMat.Succeeded())
    {
        DirtMaterial = DirtMat.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load DirtMaterial"));
    }

    if (StoneMat.Succeeded())
    {
        StoneMaterial = StoneMat.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load StoneMaterial"));
    }

    // Initialize the material mapping
    BlockTypeMaterials.Add(EBlock::Grass, GrassMaterial);
    BlockTypeMaterials.Add(EBlock::Dirt, DirtMaterial);
    BlockTypeMaterials.Add(EBlock::Stone, StoneMaterial);
    
}




int ABaseChunk::GetBlockIndex(int X, int Y, int Z) const
{
    return Z * Size.X * Size.Y + Y * Size.X + X;
}

void ABaseChunk::BeginPlay()
{
    Blocks.Init(EBlock::Air, Size.X * Size.Y * Size.Z);
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
// BaseChunk.cpp

UMaterialInterface* ABaseChunk::GetMaterialForBlockType(EBlock BlockType) const
{
    UMaterialInterface* const* FoundMaterial = BlockTypeMaterials.Find(BlockType);
    if (FoundMaterial)
    {
        return *FoundMaterial;
    }
    return nullptr;
}

void ABaseChunk::ApplyMesh()
{
    Mesh->ClearAllMeshSections();

    int32 SectionIndex = 0;
    for (const auto& Pair : MeshData.Sections)
    {
        EBlock BlockType = Pair.Key;
        const FChunkMeshSection& Section = Pair.Value;

        // Create mesh section
        Mesh->CreateMeshSection(
            SectionIndex,
            Section.Vertices,
            Section.Triangles,
            Section.Normals,
            Section.UV0,
            TArray<FColor>(),
            TArray<FProcMeshTangent>(),
            true);

        // Assign material to the section
        UMaterialInterface* Material = GetMaterialForBlockType(BlockType);
        if (Material)
        {
            Mesh->SetMaterial(SectionIndex, Material);
        }

        SectionIndex++;
    }
}

void ABaseChunk::LoadChunkData()
{

    FVector Location = GetActorLocation();

    for (int x = 0; x < Size.X; x++)
    {
        for (int y = 0; y < Size.Y; y++)
        {
            const float Xpos = (x * Scale + Location.X) / Scale;
            const float Ypos = (y * Scale + Location.Y) / Scale;

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

void ABaseChunk::Init(AChunkWorld* W)
{
    this->World = W;
}
