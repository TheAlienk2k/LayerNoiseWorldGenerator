#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include"scenes\Scene.h"
#include <memory>

class SceneManager
{
private:
	std::unique_ptr<Scene> currentScene;

public:
	SceneManager() = default;
	~SceneManager() = default;

	//Metoda służoca do ustawiania aktualnej sceny przyjmująca wskaźnik do obiektu sceny jako parametr. Ta metoda będzie odpowiedzialna za przełączanie między różnymi scenami w grze
	void setScene(std::unique_ptr<Scene> newScene);

	//Metoda służoca do aktualizacji logiki aktualnej sceny przyjmująca czas deltaTime jako parametr który reprezentuje czas jaki upłynął od ostatniej aktualizacji. Ta metoda będzie wywoływana w każdej klatce aby zapewnić płynną aktualizację logiki gry
	void update(float deltaTime);

	//Metoda służoca do renderowania aktualnej sceny. Ta metoda będzie wywoływana w każdej klatce aby narysować aktualny stan gry na ekranie
	void render();
};

