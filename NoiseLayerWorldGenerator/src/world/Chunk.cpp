#include "world\Chunk.h"

// Konstruktor domyślny inicjalizujący wskaźnik na tablicę bloków jako nullptr i ustawiający fillBlockID na 0 (co ma reprezentować "Air"!!!)
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
	//Sprawdzamy czy tablica bloków jest zainicjalizowana. Jeśli nie jest to znaczy że
	// cały chunk jest wypełniony blokiem o ID fillBlockID więc zwracamy tę wartość
	if (blocksTable == nullptr) { return fillBlockID; }

	//Zwracamy ID bloku z tablicy na podstawie jego pozycji w sekcji. Indeks obliczamy jako (x + y*16 + z*256)
	return blocksTable[x + (y << 4) + (z << 8)];
}

