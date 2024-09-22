// GreedyChunk.cpp
#include "GreedyChunk.h"

bool AGreedyChunk::CompareMask(FMask M1, FMask M2) const
{
    return M1.Block == M2.Block && M1.Normal == M2.Normal;
}

void AGreedyChunk::GenerateMesh()
{
	for (int Axis = 0; Axis < 3; ++Axis)
	{
		const int32 Axis1 = (Axis + 1) % 3;
		const int32 Axis2 = (Axis + 2) % 3;

		const int32 MainAxisLimit = Size[Axis];
		const int32 Axis1Limit = Size[Axis1];
		const int32 Axis2Limit = Size[Axis2];

		auto DeltaAxis1 = FIntVector::ZeroValue;
		auto DeltaAxis2 = FIntVector::ZeroValue;

		auto ChunkItr = FIntVector::ZeroValue;
		auto AxisMask = FIntVector::ZeroValue;

		AxisMask[Axis] = 1;

		TArray<FMask> Mask;
		Mask.SetNum(Axis1Limit * Axis2Limit);

		for (ChunkItr[Axis] = -1; ChunkItr[Axis] < MainAxisLimit;)
		{
			int32 N = 0;

			for (ChunkItr[Axis2] = 0; ChunkItr[Axis2] < Axis2Limit; ++ChunkItr[Axis2])
			{
				for (ChunkItr[Axis1] = 0; ChunkItr[Axis1] < Axis1Limit; ++ChunkItr[Axis1])
				{
					const auto CurrentBlock = GetBlock(ChunkItr);
					const auto CompareBlock = GetBlock(ChunkItr + AxisMask);

					const bool CurrenBlockOpaque = CurrentBlock != EBlock::Air;
					const bool CompareBlockOpaque = CompareBlock != EBlock::Air;

					if (CurrenBlockOpaque == CompareBlockOpaque)
					{
						Mask[N++] = {EBlock::Null, 0};
					}
					else if (CurrenBlockOpaque)
					{
						Mask[N++] = FMask{CurrentBlock, 1};
					}
					else
					{
						Mask[N++] = FMask{CompareBlock, -1};
					}
				}
			}

			++ChunkItr[Axis];
			N = 0;

			// Greedy meshing

			for (int32 j = 0; j < Axis2Limit; ++j)
			{
				for (int32 i = 0; i < Axis1Limit;)
				{
					if (Mask[N].Normal != 0)
					{
						const auto CurrentMask = Mask[N];
						ChunkItr[Axis1] = i;
						ChunkItr[Axis2] = j;

						int32 Width;
						for (Width = 1; i + Width < Axis1Limit && CompareMask(CurrentMask, Mask[N + Width]); ++Width)
							;
						;

						int32 Height;
						bool Done = false;

						for (Height = 1; j + Height < Axis2Limit; ++Height)
						{
							for (int32 k = 0; k < Width; ++k)
							{
								if (CompareMask(Mask[N + k + Height * Axis1Limit], CurrentMask))
									continue;
								Done = true;
								break;
							}
							if (Done)
							{
								break;
							}
						}

						DeltaAxis1[Axis1] = Width;
						DeltaAxis2[Axis2] = Height;

						CreateQuad(CurrentMask,
								AxisMask,
								Width,
								Height,
								ChunkItr,
								ChunkItr + DeltaAxis1,
								ChunkItr + DeltaAxis2,
								ChunkItr + DeltaAxis1 + DeltaAxis2);

						DeltaAxis1 = FIntVector::ZeroValue;
						DeltaAxis2 = FIntVector::ZeroValue;

						for (int32 l = 0; l < Height; ++l)
						{
							for (int32 k = 0; k < Width; ++k)
							{
								Mask[N + k + l * Axis1Limit] = {EBlock::Null, 0};
							}
						}

						i += Width;
						N += Width;
					}
					else
					{
						++i;
						++N;
					}
				}
			}
		}
	}
}
void AGreedyChunk::AdjustUVsForOrientation(
    FVector2D& UV1,
    FVector2D& UV2,
    FVector2D& UV3,
    FVector2D& UV4,
    const FIntVector& AxisMask,
    int32 NormalDirection)
{
    // Determine U and V axes based on the normal
    int UAxis = 0;
    int VAxis = 0;

    if (AxisMask.X != 0)
    {
        // X-axis normal, so U and V are Y and Z
        UAxis = 1;
        VAxis = 2;
    }
    else if (AxisMask.Y != 0)
    {
        // Y-axis normal, so U and V are X and Z
        UAxis = 0;
        VAxis = 2;
    }
    else if (AxisMask.Z != 0)
    {
        // Z-axis normal, so U and V are X and Y
        UAxis = 0;
        VAxis = 1;
    }

    // Swap UVs if normal is negative
    if (NormalDirection == -1)
    {
        Swap(UV1, UV2);
        Swap(UV3, UV4);
    }
}

void AGreedyChunk::CreateQuad(
    FMask Mask,
    FIntVector AxisMask,
    const int Width,
    const int Height,
    FIntVector V1,
    FIntVector V2,
    FIntVector V3,
    FIntVector V4)
{
    const auto Normal = FVector(AxisMask * Mask.Normal);

    // Get the mesh section for this block type
    FChunkMeshSection& MeshSection = MeshData.Sections.FindOrAdd(Mask.Block);

    // Add vertices
    MeshSection.Vertices.Add(FVector(V1) * Scale);
    MeshSection.Vertices.Add(FVector(V2) * Scale);
    MeshSection.Vertices.Add(FVector(V3) * Scale);
    MeshSection.Vertices.Add(FVector(V4) * Scale);

    // Add triangles with correct winding order
    if (Mask.Normal == 1)
    {
        MeshSection.Triangles.Add(MeshSection.VertexCount + 0);
        MeshSection.Triangles.Add(MeshSection.VertexCount + 2);
        MeshSection.Triangles.Add(MeshSection.VertexCount + 1);

        MeshSection.Triangles.Add(MeshSection.VertexCount + 1);
        MeshSection.Triangles.Add(MeshSection.VertexCount + 2);
        MeshSection.Triangles.Add(MeshSection.VertexCount + 3);
    }
    else
    {
        MeshSection.Triangles.Add(MeshSection.VertexCount + 0);
        MeshSection.Triangles.Add(MeshSection.VertexCount + 1);
        MeshSection.Triangles.Add(MeshSection.VertexCount + 2);

        MeshSection.Triangles.Add(MeshSection.VertexCount + 1);
        MeshSection.Triangles.Add(MeshSection.VertexCount + 3);
        MeshSection.Triangles.Add(MeshSection.VertexCount + 2);
    }

    // Add normals
    MeshSection.Normals.Add(Normal);
    MeshSection.Normals.Add(Normal);
    MeshSection.Normals.Add(Normal);
    MeshSection.Normals.Add(Normal);

    // Calculate UVs based on quad size and orientation
    FVector2D UV0(0.0f, 0.0f);
    FVector2D UV1(Width * 1.0f, 0.0f);
    FVector2D UV2(0.0f, Height * 1.0f);
    FVector2D UV3(Width * 1.0f, Height * 1.0f);

    // Correct UV mapping based on face orientation
    if (AxisMask.X != 0)
    {
        // Quad is on YZ plane
        // U corresponds to Y-axis (Axis1)
        // V corresponds to Z-axis (Axis2)
        UV0 = FVector2D(0.0f, 0.0f);
        UV1 = FVector2D(Width * 1.0f, 0.0f);
        UV2 = FVector2D(0.0f, Height * 1.0f);
        UV3 = FVector2D(Width * 1.0f, Height * 1.0f);
    }
    else if (AxisMask.Y != 0)
    {
        // Quad is on XZ plane
        // U corresponds to X-axis (Axis1)
        // V corresponds to Z-axis (Axis2)
        UV0 = FVector2D(0.0f, 0.0f);
        UV1 = FVector2D(Width * 1.0f, 0.0f);
        UV2 = FVector2D(0.0f, Height * 1.0f);
        UV3 = FVector2D(Width * 1.0f, Height * 1.0f);
    }
    else if (AxisMask.Z != 0)
    {
        // Quad is on XY plane
        // U corresponds to X-axis (Axis1)
        // V corresponds to Y-axis (Axis2)
        UV0 = FVector2D(0.0f, 0.0f);
        UV1 = FVector2D(Width * 1.0f, 0.0f);
        UV2 = FVector2D(0.0f, Height * 1.0f);
        UV3 = FVector2D(Width * 1.0f, Height * 1.0f);
    }

    // Swap UVs if normal is negative to correct texture orientation
    if (Mask.Normal == -1)
    {
        Swap(UV0, UV1);
        Swap(UV2, UV3);
    }

    // Add UVs
    MeshSection.UV0.Add(UV0);
    MeshSection.UV0.Add(UV1);
    MeshSection.UV0.Add(UV2);
    MeshSection.UV0.Add(UV3);

    // Update VertexCount
    MeshSection.VertexCount += 4;
}


void AGreedyChunk::ModifyVoxelData(const FIntVector Position, EBlock ModifyTo)
{
    const int32 Index = GetBlockIndex(Position.X, Position.Y, Position.Z);
    Blocks[Index] = ModifyTo;

    UE_LOG(LogTemp, Warning, TEXT("Modifying Voxel at Position: %s to %d"), *Position.ToString(), static_cast<uint8>(ModifyTo));

    // Regenerate mesh for the modified voxel
    ClearMesh();
    GenerateMesh();
    ApplyMesh();
}
