#include "managers/WindowManager.h"

WindowManager::WindowManager() :
	mainWindow(nullptr)
{
}

WindowManager& WindowManager::getInstance()
{
	static WindowManager instance;
	return instance;
}

void WindowManager::setMainWindow(GLFWwindow* window) {
	mainWindow = window;
}

GLFWwindow* WindowManager::getMainWindow() {
	if (!mainWindow) {
		std::cout << "[ERROR::WINDOWMANAGER] -> Attempting to access a window before its created" << std::endl;                                                                          //!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	return mainWindow;
}