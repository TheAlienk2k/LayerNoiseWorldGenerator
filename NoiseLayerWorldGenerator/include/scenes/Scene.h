#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "managers/WindowManager.h"

class Scene{
protected:
	GLFWwindow* window;

public:
	//Metoda służoca do inicjalizacji niezbędnych zasobów przed rozpoczęciem renderowania sceny
	virtual void onEnter() = 0;
	//Metoda służoca do zwalniania zasobów i wykonywania czyszczenia po zakończeniu renderowania sceny
	virtual void onExit() = 0;
	//Metoda służoca do aktualizacji logiki sceny przyjmująca czas deltaTime jako parametr który reprezentuje czas jaki upłynął od ostatniej aktualizacji
	virtual void onUpdate(float deltaTime) = 0;
	//Metoda służoca do renderowania sceny która będzie wywoływana w każdej klatce aby narysować aktualny stan sceny na ekranie
	virtual void render() = 0;
	//Opcjonalna metoda służoca do renderowania interfejsu użytkownika za pomocą ImGui która będzie wywoływana w każdej klatce po renderowaniu sceny aby narysować elementy interfejsu użytkownika na ekranie
	virtual void onImGuiRender() {};

	Scene();
	virtual ~Scene() {};
};