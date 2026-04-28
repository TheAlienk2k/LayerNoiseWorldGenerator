#include<cmath>
#include<algorithm>
#include<vector>
#include<random>
#include<numeric>
#include<iostream>
#include<iomanip>
#include<fstream>

struct vector2;

class PerlinNoise2D
{
public:
	float frequencyPerlinNoise; // 
	float amplitudePerlinNoise; //
	int octavesPerlinNoise; // Ilosc Oktaw -> bardziej 
	float frequencyChange; // Decyduje jak zmienia sie frequency po kazdej oktawie
	float amplitudeChange; // Decyduje jak zmienia sie amplitude po kazdej oktawie

	PerlinNoise2D(unsigned seed=1234, float frequency=1.0f, float amplitude=1.0f, int ocatves=4, float freqchange=2.0f, float ampchange=0.5f); // KONSTRUKTOR
	float PerlinNoiseFunction(float x, float y);	
	float DotProduct(float ix, float iy, float x, float y);
	float smoothInterpolation(float t, float x, float y);

	private:
		std::vector<int> PermutationPerlin;
};


