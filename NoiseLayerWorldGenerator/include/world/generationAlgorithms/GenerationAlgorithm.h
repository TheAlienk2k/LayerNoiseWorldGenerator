#pragma once
#include <string>
#include <imgui.h>

class GenerationAlgorithm
{
public:
	std::string layerName;

	int startY;
	int endY;

	GenerationAlgorithm(std::string layerName, int startY, int endY);
	virtual ~GenerationAlgorithm() = default;

	virtual void applyToColumn() = 0;

	virtual void renderImGuiSettings() = 0;
};

