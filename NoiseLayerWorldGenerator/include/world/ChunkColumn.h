#pragma once
#include "world\Chunk.h"
#include <vector>

#include "WorldConfig.h"

class World;
class Shader;

class ChunkColumn
{
private:
	int columnX;
	int columnZ;

	bool isMeshGenerated = false;

	std::vector<std::unique_ptr<Chunk>> chunks;

public:
	ChunkColumn(int x, int z);
	~ChunkColumn() = default;

	void setBlock(int x, int y, int z, BlockID blockID);
	BlockID getBlock(int x, int y, int z) const;

	Chunk* getChunk(int yIndex) const;

	void generateMeshes(const World* world);
	void render(Shader* shader) const;

	int getX() const;
	int getZ() const;

	bool hasMesh() const;
};

