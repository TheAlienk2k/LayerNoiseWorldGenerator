#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

// WindowManager jest singletonem który zarządza głównym oknem aplikacji. Zapewnia globalny dostęp do okna co jest przydatne w różnych częściach programu które potrzebują interakcji z oknem
class WindowManager
{
private:
	GLFWwindow* mainWindow;

	// Konstruktor jest prywatny aby uniemożliwić tworzenie instancji klasy z zewnątrz, co jest kluczowe
	WindowManager();
	// Usuwamy konstruktor kopiujący aby zapobiec kopiowaniu instancji singletona
	WindowManager(const WindowManager&) = delete;

public:
	// Metoda statyczna do uzyskania dostępu do jedynej instancji WindowManagera. Jeśli instancja jeszcze nie istnieje, zostanie utworzona przy pierwszym wywołaniu tej metody
	static WindowManager& getInstance();

	// Metoda do ustawienia wskaźnika na główne okno aplikacji. Powinna być wywołana zaraz po utworzeniu okna w funkcji main!
	void setMainWindow(GLFWwindow* window);

	// Metoda do uzyskania wskaźnika na główne okno aplikacji. Jeśli okno nie zostało jeszcze utworzone, metoda ta zgłosi błąd w konsoli						                                    !!!!!!!!!!!!!!!!!!!!!!!!!
	GLFWwindow* getMainWindow();
};

