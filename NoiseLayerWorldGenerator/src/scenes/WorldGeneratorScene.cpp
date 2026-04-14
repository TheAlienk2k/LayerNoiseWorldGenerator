#include "scenes\WorldGeneratorScene.h"


void WorldGeneratorScene::onEnter()
{
	std::cout << "+[Scene] LOADED: WorldGeneratorScene" << std::endl;

    if (window) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
		// Jeśli okno nie jest dostępne w przyszłości dodać implementacje obsługi tego przypadku np przez rzucenie wyjątku.                                                         !!!!!!!!!!!!!!!!!!!!
    }

	// Inicjalizacja kamery na pozycji (0, 0, 1) - tymczasowa wartość później bedzie trzeba ja ustalać wzgledem wytworzonego terenu
	camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 1.0f));

	//TYMCZASOWO - test ładowania shaderów
    mainShader = std::make_unique<Shader>("shaders/test.vert", "shaders/test.frag");
    floorShader = std::make_unique<Shader>("shaders/test.vert", "shaders/test.frag");

    std::vector<float> cubeVertices = {
    -0.5f, -0.5f,  0.5f, 
     0.5f, -0.5f,  0.5f, 
     0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 
    -0.5f, -0.5f, -0.5f, 
     0.5f, -0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f, 
    -0.5f,  0.5f, -0.5f  
    };

    std::vector<unsigned int> cubeIndices = {
    0, 1, 2, 2, 3, 0, 
    1, 5, 6, 6, 2, 1, 
    7, 6, 5, 5, 4, 7, 
    4, 0, 3, 3, 7, 4, 
    4, 5, 1, 1, 0, 4, 
    3, 2, 6, 6, 7, 3  
    };

    std::vector<float> floorVertices = {
        -50.0f, -1.0f, -50.0f,
         50.0f, -1.0f, -50.0f,
         50.0f, -1.0f,  50.0f,

         50.0f, -1.0f,  50.0f,
         -50.0f, -1.0f,  50.0f,
         -50.0f, -1.0f, -50.0f
    };

    floorMesh = std::make_unique<Mesh>(floorVertices);

    testMesh = std::make_unique<Mesh>(cubeVertices, cubeIndices);
    //---
}

void WorldGeneratorScene::onExit()
{
    if (window) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

	std::cout << "-[Scene] UNLOADED: WorldGeneratorScene" << std::endl;
}

void WorldGeneratorScene::onUpdate(float deltaTime)
{
	if (!camera || !window) { return; }

    if (Input::isKeyPressed(GLFW_KEY_W)) camera->processKeyboardInput(Camera::FORWARD, deltaTime);
    if (Input::isKeyPressed(GLFW_KEY_S)) camera->processKeyboardInput(Camera::BACKWARD, deltaTime);
    if (Input::isKeyPressed(GLFW_KEY_A)) camera->processKeyboardInput(Camera::LEFT, deltaTime);
    if (Input::isKeyPressed(GLFW_KEY_D)) camera->processKeyboardInput(Camera::RIGHT, deltaTime);
    if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT)) camera->processKeyboardInput(Camera::DOWN, deltaTime);
    if (Input::isKeyPressed(GLFW_KEY_SPACE)) camera->processKeyboardInput(Camera::UP, deltaTime);

    glm::vec2 delta = Input::getMouseDelta();
    camera->processMouseMovement(delta.x, delta.y);
}

void WorldGeneratorScene::render()
{
	// Ustawienie macierzy widoku kamery w shaderze
	glm::mat4 view = camera->getViewMatrix();
	mainShader->setMatrix4("view", view);

    //TYMCZASOWO - test ładowania shaderów
    if (floorShader) {
        floorShader->useShader(); 

        int colorLoc = glGetUniformLocation(floorShader->getID(), "objectColor");
        glUniform3f(colorLoc, 0.2f, 0.5f, 0.2f); 

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        floorShader->setMatrix4("projection", projection);
        floorShader->setMatrix4("view", camera->getViewMatrix());
        floorShader->setMatrix4("model", glm::mat4(1.0f));

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (floorMesh) floorMesh->draw();
    }

    if (mainShader) {
        mainShader->useShader();
        glLineWidth(3.0f);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        mainShader->setMatrix4("projection", projection);
        mainShader->setMatrix4("view", camera->getViewMatrix());

        float time = (float)glfwGetTime();
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f));
        mainShader->setMatrix4("model", model);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (testMesh) testMesh->draw();
        glLineWidth(1.0f);
    }
    //---
}

void WorldGeneratorScene::onImGuiRender()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.3f);

    if (ImGui::Begin("HUD", nullptr, window_flags)) {
        if (camera) {
            ImGui::Text("X: %.2f", camera.get()->position.x);
            ImGui::SameLine();
            ImGui::Text(" | ");
            ImGui::SameLine();
            ImGui::Text("Y: %.2f", camera.get()->position.y);
            ImGui::SameLine();
            ImGui::Text(" | ");
            ImGui::SameLine();
            ImGui::Text("Z: %.2f", camera.get()->position.z);
        }
        ImGui::Separator();
    }
    ImGui::End();

}