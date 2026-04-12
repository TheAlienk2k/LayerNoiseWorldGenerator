#include "Input.h"


bool Input::isKeyPressed(int key)
{
	return InputManager::isKeyPressed(key);
}

bool Input::isMouseButtonPressed(int button)
{
	return InputManager::isMouseButtonPressed(button);
}

glm::vec2 Input::getMousePosition()
{
	return InputManager::getMousePosition();
}

glm::vec2 Input::getMouseDelta()
{
	return InputManager::getMouseDelta();
}