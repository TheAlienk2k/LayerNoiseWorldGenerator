#pragma once
#include "Mesh.h"
#include "Shader.h" 
#include "BlockType.h"
#include <vector>
#include <memory>
#include <cstdint>

class World;
class Chunk;

class ChunkColumn
{
private:
	int columnX;
	int columnZ;

	std::unique_ptr<Mesh> columnMesh;

	bool isMeshGenerated = false;
	bool isRerenderNeeded = false;

	std::vector<std::unique_ptr<Chunk>> chunks;

public:
	ChunkColumn(int x, int z);
	~ChunkColumn() = default;

	void setBlock(int x, int y, int z, BlockID blockID);
	BlockID getBlock(int x, int y, int z) const;

	Chunk* getChunk(int yIndex) const;

	void generateMeshes(const World& world);
	void render(Shader* shader) const;

	int getX() const;
	int getZ() const;

	bool hasMesh() const;
	bool needsRerender() const;
};

