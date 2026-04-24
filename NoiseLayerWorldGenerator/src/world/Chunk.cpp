#include "world\Chunk.h"

const int Chunk::neighborOffsets[6][3] = {
		{  0,  0,  1 }, // Przód 
		{  0,  0, -1 }, // Tył
		{ -1,  0,  0 }, // Lewo
		{  1,  0,  0 }, // Prawo
		{  0,  1,  0 }, // Góra
		{  0, -1,  0 }  // Dół
};

const float Chunk::faceVertices[6][12] = {
		{ 0,0,1, 1,0,1, 1,1,1, 0,1,1 }, // Przód
		{ 1,0,0, 0,0,0, 0,1,0, 1,1,0 }, // Tył
		{ 0,0,0, 0,0,1, 0,1,1, 0,1,0 }, // Lewo
		{ 1,0,1, 1,0,0, 1,1,0, 1,1,1 }, // Prawo
		{ 0,1,1, 1,1,1, 1,1,0, 0,1,0 }, // Góra
		{ 0,0,0, 1,0,0, 1,0,1, 0,0,1 }  // Dół
};

// Konstruktor domyślny inicjalizujący wskaźnik na tablicę bloków jako nullptr i ustawiający fillBlockID na 0 (co ma reprezentować PUSTY BLOK !!!)
Chunk::Chunk() : blocksTable(nullptr), fillBlockID(0)
{

}

Chunk::~Chunk()
{
		delete[] blocksTable;
}

void Chunk::setBlock(int x, int y, int z, BlockID blockID)
{
	//Sprawdzamy czy tablica bloków jest już zainicjalizowana
	if (blocksTable == nullptr) {
		//Jeśli nie jest a ID bloku który chcemy ustawić jest taki sam jak fillBlockID to
		// znaczy że nie musimy tworzyć tablicy bo cały chunk jest już wypełniony tym blokiem
		if (blockID == fillBlockID) { return; }

		//Jeśli ID bloku jest różne od fillBlockID to musimy stworzyć tablicę i wypełnić ją wartością fillBlockID
		blocksTable = new BlockID[CHUNK_VOLUME];
		for(int i = 0; i < CHUNK_VOLUME; i++) {
			blocksTable[i] = fillBlockID;
		}
	}

	//Ustawiamy ID bloku w tablicy na podstawie jego pozycji w sekcji. Indeks obliczamy jako (x + y*16 + z*256)
	blocksTable[x + (y << 4) + (z << 8)] = blockID;
}

BlockID Chunk::getBlock(int x, int y, int z) const
{
	//Sprawdzamy czy podane współrzędne są poza granicami sekcji. Jeśli tak to zwracamy 0 (PUSTY BLOK). Musimy to zrobić ponieważ metoda ta jest używana przez metodę generateMesh
	if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) { return 0; }

	//Sprawdzamy czy tablica bloków jest zainicjalizowana. Jeśli nie jest to znaczy że
	// cały chunk jest wypełniony blokiem o ID fillBlockID więc zwracamy tę wartość
	if (blocksTable == nullptr) { return fillBlockID; }

	//Zwracamy ID bloku z tablicy na podstawie jego pozycji w sekcji. Indeks obliczamy jako (x + y*16 + z*256)
	return blocksTable[x + (y << 4) + (z << 8)];
}

BlockID Chunk::getBlockFromNeighbor(Chunk* neighbor, int x, int y, int z) const
{
	// Jeśli sąsiad istnieje to pobieramy ID bloku z tego sąsiada na podstawie podanych współrzędnych. Jeśli sąsiad nie istnieje to zwracamy 0 (PUSTY BLOK)
	if (neighbor) {
		return neighbor->getBlock(x, y, z);
	}
	return 0;
}

void Chunk::generateMesh(Chunk* topNeighbor, Chunk* bottomNeighbor, Chunk* frontNeighbor, Chunk* backNeighbor, Chunk* leftNeighbor, Chunk* rightNeighbor)
{
	// Usuwamy starą siatkę jeśli istnieje
	mesh.reset();

	// Wektory do przechowywania danych wierzchołków i indeksów dla siatki
	std::vector<float> vertices;
	std::vector<uint32_t> indices;

	// Zmienna do śledzenia aktualnego offsetu indeksów podczas generowania siatki
	uint32_t indexOffset = 0;

	//OPTYMALIZACAJ aby uniknąć wielokrotnych alokacji pamięci podczas dodawania wierzchołków i indeksów, rezerwujemy odpowiednią ilość pamięci w wektorach
	vertices.reserve(CHUNK_VOLUME * 12); //zakładamy że zazwyczaj w chunku każdy blok będzie miał około 1 widoczną ściankę, a każda ścianka ma 4 wierzchołki po 3 współrzędne czyli 12
	indices.reserve(CHUNK_VOLUME * 6 ); //dla jednej ścianki mamy 2 trójkąty po 3 indeksy czyli 6 indeksów

	// Iterujemy przez wszystkie bloki w sekcji
	for (int y = 0; y < CHUNK_SIZE; y++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int x = 0; x < CHUNK_SIZE; x++) {

				// Pobieramy ID aktualnego bloku. Jeśli jest równy 0 (Pusty blok) to pomijamy go i przechodzimy do następnego bloku
				BlockID currentBlockID = getBlock(x, y, z);
				if (currentBlockID == 0) { continue; }

				// Dla każdego bloku sprawdzamy jego 6 sąsiadów (przód, tył, lewo, prawo, góra, dół) aby określić które ścianki są widoczne
				for (int wall = 0; wall < 6; wall++) {
					// Obliczamy pozycję sąsiada na podstawie aktualnej pozycji bloku i kierunku sąsiada
					int neighborX = x + neighborOffsets[wall][0];
					int neighborY = y + neighborOffsets[wall][1];
					int neighborZ = z + neighborOffsets[wall][2];

					// Zakładamy że ścianka nie jest widoczna dopóki nie sprawdzimy sąsiada
					bool isFaceVisible = false;

					//Zmienna przechowuje ID bloku sąsiadującego z aktualną ścianą
					BlockID neighborID = 0;

					//Logika sprawdzania widoczności ścianki wzgledem sąsiada
					if (neighborY >= CHUNK_SIZE) {
						//Sąsiad powyżej aktualnego chunka - bierzemy blok z samego dołu (Y=0) górnego sąsiada
						neighborID = getBlockFromNeighbor(topNeighbor, neighborX, 0, neighborZ);
					}
					else if (neighborY < 0) {
						//Sąsiad poniżej aktualnego chunka - bierzemy blok z samej góry (Y=MAX) dolnego sąsiada
						neighborID = getBlockFromNeighbor(bottomNeighbor, neighborX, CHUNK_SIZE - 1, neighborZ);
					}
					else if (neighborX >= CHUNK_SIZE) {
						//Sąsiad po prawej stronie - bierzemy blok z lewej krawędzi (X=0) prawego sąsiada
						neighborID = getBlockFromNeighbor(rightNeighbor, 0, neighborY, neighborZ);
					}
					else if (neighborX < 0) {
						//Sąsiad po lewej stronie - bierzemy blok z prawej krawędzi (X=MAX) lewego sąsiada
						neighborID = getBlockFromNeighbor(leftNeighbor, CHUNK_SIZE - 1, neighborY, neighborZ);
					}
					else if (neighborZ >= CHUNK_SIZE) {
						//Sąsiad z przodu - bierzemy blok z tyłu (Z=0) przedniego sąsiada
						neighborID = getBlockFromNeighbor(frontNeighbor, neighborX, neighborY, 0);
					}
					else if (neighborZ < 0) {
						//Sąsiad z tyłu - bierzemy blok z przodu (Z=MAX) tylnego sąsiada
						neighborID = getBlockFromNeighbor(backNeighbor, neighborX, neighborY, CHUNK_SIZE - 1);
					}
					else {
						//Sąsiad znajduje się wewnątrz tego samego chunka - pobieramy go bezpośrednio
						neighborID = getBlock(neighborX, neighborY, neighborZ);
					}


					//Decyzja o rysowaniu ścianki na podstawie otrzymanego bloku sąsiada
					if (neighborID == 0) {
						//Jeśli sąsiad jest pustym blokiem (ID=0)
						isFaceVisible = true;
					}
					else if (BlockDatabase::getBlockData(neighborID).isTransparent) {
						//Jęśli sąsiad jest przeźroczysty
						isFaceVisible = true;
					}

					// Jeśli ścianka jest widoczna to dodajemy jej wierzchołki do wektora wierzchołków
					if (isFaceVisible) {

						// Dodajemy wierzchołki dla tej ścianki. Współrzędne wierzchołków są obliczane na podstawie lokalnych współrzędnych ścianki (faceVertices) i globalnej pozycji bloku (x, y, z)
						for (int v = 0; v < 4; v++) {
							// Dodajemy globalne współrzędne wierzchołka do wektora wierzchołków. Współrzędne lokalne ścianki są dodawane do globalnej pozycji bloku aby uzyskać globalne współrzędne wierzchołka
							vertices.push_back(x + faceVertices[wall][v * 3 + 0]);
							vertices.push_back(y + faceVertices[wall][v * 3 + 1]);
							vertices.push_back(z + faceVertices[wall][v * 3 + 2]);
						}

						// Dodajemy indeksy dla tej ścianki (2 trójkąty tworzące kwadrat)
						indices.push_back(indexOffset + 0);
						indices.push_back(indexOffset + 1);
						indices.push_back(indexOffset + 2);

						indices.push_back(indexOffset + 2);
						indices.push_back(indexOffset + 3);
						indices.push_back(indexOffset + 0);

						// Aktualizujemy offset indeksów o 4 ponieważ każda ścianka dodaje 4 wierzchołki
						indexOffset += 4;
					}

				}

			}
		}
		
		// Po zakończeniu iteracji przez wszystkie bloki ustawiamy flagę isMeshGenerated na true aby oznaczyć że siatka została wygenerowana
		isMeshGenerated = true;
	}

	// Jeśli wygenerowano jakieś wierzchołki to tworzymy siatkę na podstawie tych wierzchołków i indeksów
	if (!vertices.empty()) {
		mesh = std::make_unique<Mesh>(vertices, indices);
	}
}

//
void Chunk::render() const
{
	// Jeśli siatka została wygenerowana to rysujemy ją
	if (mesh) {
		mesh->draw();
	}
}

bool Chunk::hasMesh() const
{
	return isMeshGenerated;
}

