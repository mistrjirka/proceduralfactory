// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyChunk.h"


void AGreedyChunk::GenerateBlocks()
{
	const auto Location = GetActorLocation();

	for (int x = 0; x < Size.X; x++)
	{
		for (int y = 0; y < Size.Y; y++)
		{
			const float Xpos = (x * 100 + Location.X) / 100;
			const float Ypos = (y * 100 + Location.Y) / 100;

			const int Height = FMath::Clamp(FMath::RoundToInt((Noise->GetNoise(Xpos, Ypos) + 1) * Size.Z / 2), 0, Size.Z);

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
}
bool AGreedyChunk::CompareMask(FMask M1, FMask M2) const
{
	return M1.Block == M2.Block && M1.Normal == M2.Normal;
}

void AGreedyChunk::GenerateMesh()
{
	for(int Axis = 0; Axis < 3; ++Axis)
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

		for(ChunkItr[Axis] = -1; ChunkItr[Axis] < MainAxisLimit;)
		{
			int32 N = 0;

			for(ChunkItr[Axis2] = 0; ChunkItr[Axis2] < Axis2Limit; ++ChunkItr[Axis2])
			{
				for(ChunkItr[Axis1] = 0; ChunkItr[Axis1] < Axis1Limit; ++ChunkItr[Axis1])
				{
					const auto CurrentBlock = GetBlock(ChunkItr);
					const auto CompareBlock = GetBlock(ChunkItr + AxisMask);

					const bool CurrenBlockOpaque = CurrentBlock != EBlock::Air;
					const bool CompareBlockOpaque = CompareBlock != EBlock::Air;

					if(CurrenBlockOpaque == CompareBlockOpaque)
					{
						Mask[N++] = {EBlock::Null, 0};
					}
					else if(CurrenBlockOpaque)
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

			for(int32 j = 0; j < Axis2Limit; ++j)
			{
				for(int32 i = 0; i < Axis1Limit;)
				{
					if(Mask[N].Normal != 0)
					{
						const auto CurrentMask = Mask[N];
						ChunkItr[Axis1] = i;
						ChunkItr[Axis2] = j;

						int32 Width;
						for(Width = 1; i + Width < Axis1Limit && CompareMask(CurrentMask, Mask[N + Width]); ++Width)
						;;

						int32 Height;
						bool Done = false;

						for(Height = 1; j + Height < Axis2Limit; ++Height)
						{
							for(int32 k = 0; k < Width; ++k)
							{
								if(CompareMask(Mask[N + k + Height * Axis1Limit], CurrentMask)) continue;
								Done = true;
								break;
							}
							if(Done)
							{
								break;
							}
						}

						DeltaAxis1[Axis1] = Width;
						DeltaAxis2[Axis2] = Height;

						CreateQuad(CurrentMask, 
							AxisMask, 
							ChunkItr, 
							ChunkItr + DeltaAxis1, 
							ChunkItr + DeltaAxis2, 
							ChunkItr + DeltaAxis1 + DeltaAxis2
						);

						DeltaAxis1 = FIntVector::ZeroValue;
						DeltaAxis2 = FIntVector::ZeroValue;

						for(int32 l = 0; l < Height; ++l)
						{
							for(int32 k = 0; k < Width; ++k)
							{
								Mask[N + k + l * Axis1Limit] = {EBlock::Null, 0};
							}
						}

                        i += Width;
                        N += Width;

					} else
					{
						++i;
						++N;
					}
				}
			}

		}
	}	
}

void AGreedyChunk::CreateQuad(FMask Mask, FIntVector AxisMask, FIntVector V1, FIntVector V2, FIntVector V3, FIntVector V4)
{
	const auto Normal = FVector(AxisMask*Mask.Normal);
	MeshData.Vertices.Add(FVector(V1)*100);
	MeshData.Vertices.Add(FVector(V2)*100);
	MeshData.Vertices.Add(FVector(V3)*100);
	MeshData.Vertices.Add(FVector(V4)*100);

	MeshData.Triangles.Add(VertexCount);
	MeshData.Triangles.Add(VertexCount + 2 + Mask.Normal);
	MeshData.Triangles.Add(VertexCount + 2 - Mask.Normal);
	MeshData.Triangles.Add(VertexCount + 3);
	MeshData.Triangles.Add(VertexCount + 1 - Mask.Normal);
	MeshData.Triangles.Add(VertexCount + 1 + Mask.Normal);

	MeshData.UV0.Add(FVector2D(0, 0));
	MeshData.UV0.Add(FVector2D(0, 1));
	MeshData.UV0.Add(FVector2D(1, 0));
	MeshData.UV0.Add(FVector2D(1, 1));

	MeshData.Normals.Add(Normal);
	MeshData.Normals.Add(Normal);
	MeshData.Normals.Add(Normal);
	MeshData.Normals.Add(Normal);
	
	VertexCount += 4;

		
}

void AGreedyChunk::ModifyVoxelData(const FIntVector Position, EBlock ModifyTo)
{
	const int32 Index = GetBlockIndex(Position.X, Position.Y, Position.Z);
	Blocks[Index] = ModifyTo;

	UE_LOG(LogTemp, Warning, TEXT("Modifying Voxel at Position: %s to %d"), *Position.ToString(), static_cast<uint8>(ModifyTo));
}