#pragma once

#include "world/World.h"
#include "Camera.h"
#include "Shader.h"

class WorldRenderer
{
private:
	bool isColumnInRenderDistance(int columnX, int columnZ, int cameraColumnX, int cameraColumnZ) const;

	void renderChunkColumn(const ChunkColumn* column, Shader& shader) const;

public:
	WorldRenderer();
	~WorldRenderer() = default;

	void render(World& world, const Camera& camera, Shader& shader, float windowAspectRatio);
};

