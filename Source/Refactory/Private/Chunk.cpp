#include "Chunk.h"

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChunk::GenerateMesh()
{
	for(int x = 0; x < Size.X; ++x)
	{
		for(int y = 0; y < Size.Y; ++y)
		{
			for(int z = 0; z < Size.Z; ++z)
			{
				if(Blocks[GetBlockIndex(x, y, z)] != EBlock::Air)
				{
					const FVector Position = FVector(x, y, z);

					for (auto Direction : {EDirection::Forward, EDirection::Right, EDirection::Back, EDirection::Left, EDirection::Up, EDirection::Down})
					{
						
						if (Check(GetPositionInDirection(Direction, Position)))
						{
							CreateFace(Direction, Position * 100);
						}

					}
				}
			}
		}
	}
}

TArray<FVector> AChunk::GetFaceVertices(EDirection Direction, FVector Position) const
{
	TArray<FVector> Vertices;

	for (int i = 0; i < 4; i++)
	{
		FVector vertex = BlockVectorData[BlockTriangleData[static_cast<int>(Direction) * 4 + i]] * Scale + Position;
		Vertices.Add(vertex);
	}

	return Vertices;
}

void AChunk::CreateFace(EDirection Direction, FVector Position)
{
	MeshData.Vertices.Append(GetFaceVertices(Direction, Position));
	MeshData.UV0.Append({FVector2D(1, 1), FVector2D(1, 0), FVector2D(0, 0), FVector2D(0, 1)});
	MeshData.Triangles.Append({VertexCount + 3, VertexCount + 2, VertexCount, VertexCount + 2, VertexCount + 1, VertexCount});
	VertexCount += 4;
}

bool AChunk::Check(FVector Position) const
{
	if (Position.X >= Size.X || Position.Y >= Size.Y || Position.Z >= Size.Z || Position.X < 0 || Position.Y < 0 || Position.Z < 0)
	{
		return true;
	}

	return Blocks[GetBlockIndex(Position.X, Position.Y, Position.Z)] == EBlock::Air;
}

void AChunk::ModifyVoxelData(const FIntVector Position, EBlock ModifyTo)
{
	const int32 Index = GetBlockIndex(Position.X, Position.Y, Position.Z);
	Blocks[Index] = ModifyTo;
}

