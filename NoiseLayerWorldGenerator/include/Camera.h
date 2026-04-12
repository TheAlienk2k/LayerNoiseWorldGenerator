#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class Camera
{
private:
	// Kąty Yaw i Pitch określają orientację kamery w przestrzeni. Yaw kontroluje obrót wokół osi Y, a Pitch kontroluje obrót wokół osi X.
	float Yaw = -90.0f;
	float Pitch = 0.0f;

	// Aktualizuje wektory right i up na podstawie aktualnej pozycji i orientacji kamery
	void updateCameraVectors();

public:
	// Enum do reprezentowania kierunków ruchu kamery
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	glm::vec3 position; // Pozycja kamery w przestrzeni świata
	glm::vec3 front; // Kierunek, w którym kamera jest skierowana
	glm::vec3 up; // Wektor "góry" kamery, używany do określenia orientacji
	glm::vec3 right; // Wektor "prawo" kamery, prostopadły do front i up
	glm::vec3 worldUp; // Wektor "góry" świata, używany do obliczania orientacji kamery

	float movementSpeed;

	// Konstruktor inicjalizujący kamerę na określonej pozycji.
	Camera(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 3.0f));

	// Metoda do uzyskania macierzy widoku kamery która będzie używana w shaderze
	glm::mat4 getViewMatrix() const;

	// Metoda do przetwarzania wejścia z klawiatury i aktualizacji pozycji kamery
	void processKeyboardInput(Camera_Movement direction, float deltaTime);

	// Metoda do przetwarzania ruchu myszy i aktualizacji orientacji kamery
	void processMouseMovement(float xoffset, float yoffset);
};

