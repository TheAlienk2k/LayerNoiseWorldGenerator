#pragma once
#include "BlockType.h"
#include <Mesh.h>

class Chunk
{
private:
	// Kierunki sąsiadów (X, Y, Z)
	static const int neighborOffsets[6][3];

	// Wierzchołki dla każdej z 6 ścianek - (WSPÓŁRZĘDNE LOKALNE!)
	static const float faceVertices[6][12];

	// Tablica przechowująca ID bloków w sekcj
	BlockID* blocksTable;
	// ID bloku który będzie używany do wypełniania jeśli sekcja jest jednolita
	BlockID fillBlockID;
	//wskaźnik na siatkę reprezentującą sekcję
	std::unique_ptr<Mesh> mesh;

	// Funkcja pomocnicza do pobierania ID bloku z sąsiedniej sekcji.
	// Ta funkcja jest używana podczas generowania siatki aby sprawdzić widoczność ścianek na granicy sekcji
	BlockID getBlockFromNeighbor(Chunk* neighbor, int x, int y, int z) const;

public:
	Chunk();
	~Chunk();

	//Rozmiar sekcji zawsze będzie 16x16x16, co daje 4096 bloków
	const static int CHUNK_SIZE = 16;
	const static  int CHUNK_VOLUME = 4096;

	// Funkcja ustawiająca ID bloku na podstawie jego pozycji w sekcji
	void setBlock(int x, int y, int z, BlockID blockID);
	// Funkcja zwracająca ID bloku na podstawie jego pozycji w sekcji
	BlockID getBlock(int x, int y, int z) const;

	// Funkcja generująca siatkę na podstawie aktualnych danych bloków w sekcji. Ta funkcja powinna być wywoływana po każdej zmianie danych bloków
	void generateMesh(Chunk* topNeighbor = nullptr, Chunk* bottomNeighbor = nullptr, Chunk* frontNeighbor = nullptr, Chunk* backNeighbor = nullptr, Chunk* leftNeighbor = nullptr, Chunk* rightNeighbor = nullptr);

	//Funkcja renderująca siatkę sekcji. Ta funkcja powinna być wywoływana podczas renderowania świata
	void render() const;
};
