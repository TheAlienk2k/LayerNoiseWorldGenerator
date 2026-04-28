#include "Mesh.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
	// Obliczanie liczby wierzchołków na podstawie rozmiaru wektora wierzchołków. Zakładamy że każdy wierzchołek składa się z 3 wartości (x, y, z)
	vertexCount = static_cast<int>(vertices.size() / 3);
	// Obliczanie liczby indeksów na podstawie rozmiaru wektora indeksów
	indexCount = static_cast<int>(indices.size());

	//Zarezerwowanie pamięci GPU dla VAO i VBO które będą przechowywać dane wierzchołków i konfigurację atrybutów wierzchołków
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Aktywacja VAO co pozwala na późniejsze wiązanie tego VAO podczas renderowania aby używać jego konfiguracji atrybutów wierzchołków
	glBindVertexArray(VAO);

	//Przesłanie danych wierzchołków z CPU do GPU poprzez wiązanie VBO i kopiowanie danych wierzchołków do tego bufora, co umożliwia GPU dostęp do tych danych podczas renderowania
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// Jeśli używamy indeksowania wierzchołków to przesyłamy dane indeksów do GPU poprzez wiązanie EBO i kopiowanie danych indeksów do tego bufora, co umożliwia GPU dostęp do tych danych podczas renderowania z użyciem indeksów
	if (indexCount > 0) {
		//Zarezerwowanie pamięci GPU dla EBO który będzie przechowywał dane indeksów jeśli używamy indeksowania wierzchołków
		glGenBuffers(1, &EBO);

		//Przesłanie danych indeksów z CPU do GPU poprzez wiązanie EBO i kopiowanie danych indeksów do tego bufora, co umożliwia GPU dostęp do tych danych podczas renderowania z użyciem indeksów
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}
	else {
		// Jeśli nie używamy indeksowania wierzchołków to ustawiamy EBO na 0 co oznacza że nie będzie używany podczas renderowania
		EBO = 0;
	}

	//Informowanie OpenGL o formacie danych wierzchołków poprzez ustawienie atrybutów wierzchołków. W tym przypadku zakładamy że każdy wierzchołek składa się z 3 wartości typu float (x, y, z) i że dane są ułożone bezpośrednio po sobie w buforze
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Odpinanie VAO i VBO po zakończeniu konfiguracji
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	// Zwalnianie zasobów GPU związanych z VAO, VBO i EBO aby uniknąć wycieków pamięci GPU
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	if (EBO != 0) {
		glDeleteBuffers(1, &EBO);
	}
}

void Mesh::draw() const
{
	// Renderowanie siatki poprzez wiązanie VAO i wywołanie funkcji rysującej która wykorzystuje liczbę wierzchołków do określenia ile wierzchołków ma zostać narysowanych
	glBindVertexArray(VAO);

	// Jeśli używamy indeksowania wierzchołków (EBO) to rysujemy elementy za pomocą glDrawElements, w przeciwnym razie rysujemy bezpośrednio wierzchołki za pomocą glDrawArrays
	if (indexCount > 0) {
		// Rysowanie z indeksowaniem wierzchołków
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	} else {
		// Rysowanie bez indeksowania wierzchołków
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	// Odpinanie VAO po zakończeniu renderowania
	glBindVertexArray(0);
}
