#pragma once
#include <glad/glad.h>
#include <vector>

class Mesh
{
public:
	// Konstruktor który tworzy siatkę na podstawie podanych wierzchołków. Inicjalizuje VAO i VBO oraz ustawia atrybuty wierzchołków
	Mesh(const std::vector<float>& vertices);
	~Mesh();

	// Funkcja która renderuje siatkę. 
	void draw() const;

private:
	// Identyfikatory VAO (Vertex Array Object) i VBO (Vertex Buffer Object) używane do przechowywania danych wierzchołków na GPU
	unsigned int VAO;
	unsigned int VBO;

	// Liczba wierzchołków w siatce używana do określenia ile wierzchołków ma zostać narysowanych podczas renderowania
	int vertexCount;
};

