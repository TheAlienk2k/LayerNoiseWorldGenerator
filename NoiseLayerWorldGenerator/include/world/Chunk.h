#pragma once
#include "BlockType.h"
#include <Mesh.h>

//Rozmiar sekcji zawsze będzie 16x16x16, co daje 4096 bloków
const int CHUNK_SIZE = 16;
const int CHUNK_VOLUME = 4096;

class Chunk
{
public:
	Chunk();
	~Chunk();

	// Usunięcie konstruktora kopiującego i operatora przypisania aby uniknąć przypadkowego kopiowania obiektów (BEZPIECZEŃSTWO PAMIĘCI + ŁATWIEJSZY DEBUG)
	Chunk(const Chunk&) = delete;
	Chunk& operator=(const Chunk&) = delete;

	// Funkcja ustawiająca ID bloku na podstawie jego pozycji w sekcji
	void setBlock(int x, int y, int z, BlockID blockID);

	// Funkcja zwracająca ID bloku na podstawie jego pozycji w sekcji
	BlockID getBlock(int x, int y, int z) const;

private:
	// Tablica przechowująca ID bloków w sekcj
	BlockID* blocksTable;
	// ID bloku który będzie używany do wypełniania jeśli sekcja jest jednolita
	BlockID fillBlockID;
	//wskaźnik na siatkę reprezentującą sekcję
	std::unique_ptr<Mesh> mesh;
};

