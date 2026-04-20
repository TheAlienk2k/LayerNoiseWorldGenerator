#include "world\World.h"

World::World() {

}

void World::addChunkColumn(int x, int z) 
{
	ChunkCords position = { x, z };

	if (columnsMap.find(position) == columnsMap.end()) {
		columnsMap[position] = std::make_unique<ChunkColumn>(x, z);
	}
}

ChunkColumn* World::getChunkColumn(int x, int z) const 
{
	ChunkCords position = { x, z };
	auto it = columnsMap.find(position);

	if (it != columnsMap.end()) {
		return it->second.get();
	}

	return nullptr;
}

void World::setBlock(int x, int y, int z, BlockID blockID)
{

	if (y < 0 || y >= config.worldHeightInChunks * Chunk::CHUNK_SIZE) {
		return;
	}

	int columnX = x / Chunk::CHUNK_SIZE;
	int columnZ = z / Chunk::CHUNK_SIZE;

	ChunkColumn* column = getChunkColumn(columnX, columnZ);

	if (column) {
		int localX = x % Chunk::CHUNK_SIZE;
		int localZ = z % Chunk::CHUNK_SIZE;

		if (localX < 0) { localX += Chunk::CHUNK_SIZE; }
		if (localZ < 0) { localZ += Chunk::CHUNK_SIZE; }

		column->setBlock(localX, y, localZ, blockID);
	}
}

BlockID World::getBlock(int x, int y, int z) const 
{
	if (y < 0 || y >= config.worldHeightInChunks * Chunk::CHUNK_SIZE) {
		return 0;
	}
	
	int columnX = x / Chunk::CHUNK_SIZE;
	int columnZ = z / Chunk::CHUNK_SIZE;

	ChunkColumn* column = getChunkColumn(columnX, columnZ);

	if (column) {
		int localX = x % Chunk::CHUNK_SIZE;
		int localZ = z % Chunk::CHUNK_SIZE;

		if (localX < 0) { localX += Chunk::CHUNK_SIZE; }
		if (localZ < 0) { localZ += Chunk::CHUNK_SIZE; }

		return column->getBlock(localX, y, localZ);
	}

	return 0;
}

void World::generateWorldMesh() 
{
	for(auto const& it : columnsMap) {
		it.second->generateMeshes(this);
	}	
}

void World::render(Shader* shader) const 
{
	for (auto const& it : columnsMap) {
		it.second->render(shader);
	}
}