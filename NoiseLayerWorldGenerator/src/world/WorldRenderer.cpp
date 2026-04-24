#include "world/WorldRenderer.h"

WorldRenderer::WorldRenderer()
{

}

void WorldRenderer::render(World& world, const Camera& camera, Shader& shader, float windowAspectRatio)
{
	shader.useShader();

	shader.setMatrix4("view", camera.getViewMatrix());
	shader.setMatrix4("projection", camera.getProjectionMatrix(windowAspectRatio));

	int cameraColumnX = static_cast<int>(std::floor(camera.position.x / (Chunk::CHUNK_SIZE)));
	int cameraColumnZ = static_cast<int>(std::floor(camera.position.z / (Chunk::CHUNK_SIZE)));

	for (int x = -config.renderDistance; x <= config.renderDistance; x++) {
		for (int z = -config.renderDistance; z <= config.renderDistance; z++) {
			
			int columnX = cameraColumnX + x;
			int columnZ = cameraColumnZ + z;

			ChunkColumn* column = world.getChunkColumn(columnX, columnZ);

			if (column && column->hasMesh())
			{
				for (int y = 0; y < config.worldHeightInChunks; y++) {
					Chunk* chunk = column->getChunk(y);

					if (chunk && chunk->hasMesh()) {
						renderChunk(chunk, column->getX(), column->getZ(), y, shader);
					}
				}
			}

		}
	}
	
}

bool WorldRenderer::isColumnInRenderDistance(int columnX, int columnZ, int cameraColumnX, int cameraColumnZ) const
{
	
	if (std::abs(columnX - cameraColumnX) <= config.renderDistance &&
		std::abs(columnZ - cameraColumnZ) <= config.renderDistance) 
	{
		return true;
	}

	return false;
}

void WorldRenderer::renderChunk(const Chunk* chunk, int columnX, int columnZ, int chunkY, Shader& shader) const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(columnX * Chunk::CHUNK_SIZE, chunkY * Chunk::CHUNK_SIZE, columnZ * Chunk::CHUNK_SIZE));
	shader.setMatrix4("model", model);

	chunk->render();
}