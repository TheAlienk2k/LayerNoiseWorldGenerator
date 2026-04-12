#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices)
{
	// Obliczanie liczby wierzchołków na podstawie rozmiaru wektora wierzchołków. Zakładamy że każdy wierzchołek składa się z 3 wartości (x, y, z)
	vertexCount = static_cast<int>(vertices.size() / 3);

	//Zarezerwowanie pamięci GPU dla VAO i VBO które będą przechowywać dane wierzchołków i konfigurację atrybutów wierzchołków
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Aktywacja VAO co pozwala na późniejsze wiązanie tego VAO podczas renderowania aby używać jego konfiguracji atrybutów wierzchołków
	glBindVertexArray(VAO);

	//Przesłanie danych wierzchołków z CPU do GPU poprzez wiązanie VBO i kopiowanie danych wierzchołków do tego bufora, co umożliwia GPU dostęp do tych danych podczas renderowania
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	//Informowanie OpenGL o formacie danych wierzchołków poprzez ustawienie atrybutów wierzchołków. W tym przypadku zakładamy że każdy wierzchołek składa się z 3 wartości typu float (x, y, z) i że dane są ułożone bezpośrednio po sobie w buforze
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Odpinanie VAO i VBO po zakończeniu konfiguracji
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	// Zwalnianie zasobów GPU związanych z VAO i VBO aby uniknąć wycieków pamięci GPU
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::draw() const
{
	// Renderowanie siatki poprzez wiązanie VAO i wywołanie funkcji rysującej która wykorzystuje liczbę wierzchołków do określenia ile wierzchołków ma zostać narysowanych
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
}
