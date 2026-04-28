#include "managers\SceneManager.h"

void SceneManager::setScene(std::unique_ptr<Scene> newScene) {
	// Jeśli istnieje aktualna scena wywołujemy jej metodę OnExit() przed przełączeniem na nową scenę
	if (currentScene) {
		currentScene->onExit();
	}
	// Przypisujemy nową scenę do currentScene
	currentScene = std::move(newScene);

	// Po ustawieniu nowej sceny wywołujemy jej metodę OnEnter() w celu inicjalizacji zasobów i przygotowania do renderowania
	if (currentScene) {
		currentScene->onEnter();
	}
}

void SceneManager::update(float deltaTime) {
	// Jeśli istnieje aktualna scena wywołujemy jej metodę OnUpdate() z przekazanym czasem deltaTime aby zaktualizować logikę sceny
	if (currentScene) {
		currentScene->onUpdate(deltaTime);
	}
}

void SceneManager::render() {
	// Jeśli istnieje aktualna scena wywołujemy jej metodę Render() w celu narysowania aktualnego stanu sceny na ekranie
	if (currentScene) {
		currentScene->render();
	}

	//Incijalizujemy nową klatkę dla ImGui, co pozwala na rysowanie interfejsu użytkownika w każdej klatce po renderowaniu sceny.
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Dodanie elementów intyerfejsu dla danej sceny, jeśli metoda onImGuiRender() została nadpisana w klasie sceny.
	currentScene->onImGuiRender();

	// Kończymy klatkę ImGui i renderujemy interfejs użytkownika na ekranie
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}