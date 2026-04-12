#pragma once
#include "scenes\Scene.h"
#include <iostream>
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Input.h"
#include "managers/WindowManager.h"

class WorldGeneratorScene : public Scene
{
public:
	WorldGeneratorScene() = default;
	~WorldGeneratorScene() override = default;

	void onEnter() override;
	void onExit() override;
	void onUpdate(float deltaTime) override;
	void render() override;

	void onImGuiRender() override;

private:
	std::unique_ptr<Camera> camera;

	//TYMCZASOWO - test ładowania shaderów
	std::unique_ptr<Shader> mainShader;
	std::unique_ptr<Mesh> testMesh; 

	std::unique_ptr<Shader> floorShader;
	std::unique_ptr<Mesh> floorMesh;
	//---
};

