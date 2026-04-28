#include "PerlinNoise2D.h"



struct vector2
{
	float x, y;
}; 

vector2 gradients[8] = {
	{0,1},
	{1,1},
	{1,0},
	{0,-1},
	{-1,-1},
	{-1,0},
	{1,-1},
	{-1,1},
};

// KONSTRUKTOR - > TWORZE PERMUTACJE, Jeden seed -> jedno potasowanie. Czyli Kazdy seed bedzie odpowiadal jednemu potasowaniu tabeli Permutacji (PermutationPerlin) 
PerlinNoise2D::PerlinNoise2D(unsigned seed, float frequency, float amplitude, int octaves, float freqchange, float ampchange) : frequencyPerlinNoise(frequency), amplitudePerlinNoise(amplitude), octavesPerlinNoise(octaves), frequencyChange(freqchange), amplitudeChange(ampchange)
{
	PermutationPerlin.resize(256);
	std::iota(PermutationPerlin.begin(), PermutationPerlin.end(), 0);

	// NA PODSTAWIE SEEDA TASUJEMY NASZA TABELE PERMUTACJI 
	std::default_random_engine engine(seed);
	std::shuffle(PermutationPerlin.begin(), PermutationPerlin.end(), engine);

	// POWIELANIE TABLIY PERMUTACJI -> [0, ..., 255, 0, ..... 255], CHRONI PRZED WYJSCIEM POZA ROZMIAR WEKTORA
	PermutationPerlin.insert(PermutationPerlin.end(), PermutationPerlin.begin(), PermutationPerlin.end());
}



float PerlinNoise2D::smoothInterpolation(float t, float x, float y) ///// x,y - dotproducty poszczegolnych gridow, t - W ZALEZNOSCI ALBO ix/iy 
{
	return x + t * t * t * (t * (6.0f * t - 15.0f) + 10.0f) * (y - x);
}

float PerlinNoise2D::PerlinNoiseFunction(float x, float y)
{
	float TotalNoiseValue = 0.0;
	float frequency = frequencyPerlinNoise;
	float StandardFrequency = frequency;
	float amplitude = amplitudePerlinNoise;
	int octaves = octavesPerlinNoise;
	float freqchange = frequencyChange;
	float ampchange = amplitudeChange;

	for (int i = 0; i < octaves; i++)
	{
		//// PRZESKALOWYWUJE PUNKT PRZY UZYCIU FREQUENCY 
		float fx = x * frequency;
		float fy = y * frequency;

		/////// LICZYMY WSPOLRZEDNE GRIDÓW (x0,y0), (x0,y1), (x1,y1), (x1,y0) /////
		int x0 = static_cast<int>(floor(fx)); // Floor(x) -> ZWRACA WARTOSC CALKOWITA Z LICZBY ZMIENNOPRZECINKOWEJ
		int y0 = static_cast<int>(floor(fy)); // 
		int x1 = x0 + 1; 
		int y1 = y0 + 1; 

		float ix = fx - x0; // CZESC DZIESIETNA LICZBY x (x=2,4 -> x0 = 2 -> ix = 0,4)
		float iy = fy - y0; // CZESC DZIESIETNA LICZBY y


		///  PRZYDZIELAM  KAZDEMU GRID PUNKTOWI LICZBE Z ZAKRESU 0-255 
		int p00 = PermutationPerlin[(PermutationPerlin[x0 & 255] + y0) & 255];
		int p10 = PermutationPerlin[(PermutationPerlin[x1 & 255] + y0) & 255];
		int p01 = PermutationPerlin[(PermutationPerlin[x0 & 255] + y1) & 255];
		int p11 = PermutationPerlin[(PermutationPerlin[x1 & 255] + y1) & 255];

		/// TERAZ WYBIERAM JEDEN Z SIEDMIU GRADIENTOW na PODSTAWIE LICZBY p
		vector2 g00 = gradients[p00 & 7]; 
	    vector2 g10 = gradients[p10 & 7];
	    vector2 g01 = gradients[p01 & 7];
	    vector2 g11 = gradients[p11 & 7];


		/// LICZE DOT PRODUCT(Iloczyn Skalarny)   
		float dp00 = DotProduct(g00.x, g00.y, ix, iy);
		float dp10 = DotProduct(g10.x, g10.y, ix - 1, iy);
		float dp01 = DotProduct(g01.x, g01.y, ix, iy - 1);
		float dp11 = DotProduct(g11.x, g11.y, ix - 1, iy - 1);

		///

		float interpolationXUp = smoothInterpolation(ix, dp00, dp10); /// INTERPOLACJA POMIEDZY GORNYMI DOTPRODUCTAMI
		float interpolationXDown = smoothInterpolation(ix, dp01, dp11); /// INTERPOLACJA POMIEDZY DOLNYMI DOTPRODUCTAMI
		float interpolationValue = smoothInterpolation(iy, interpolationXUp, interpolationXDown); /// INTERPOLACJA MIEDZY NIMI
		float NoiseValue = interpolationValue;
	
		TotalNoiseValue += NoiseValue*amplitude;
		// ZMIENIA FREQUENCY I AMPLITUDE NA KOLEJNA OKTAWE
		frequency *= freqchange; 
		amplitude *= ampchange;
	}

	return TotalNoiseValue;
  }

// DOTPRODUCT -> ILOCZYN SKALARNY WEKTOROW. 
// gx,gy => WSPOLRZEDNE WEKTORA GRADIENTU. 
// x,y => WSPOLRZEDNE WEKTORA OD PUNKTU GRID DO NASZEGO PUNKTA (ix,iy) -> CZYLI CZESCI DZIESIETNEJ PUNKTU (x,y)
float PerlinNoise2D::DotProduct(float gx, float gy, float x, float y) 
{
	return gx * x + gy * y;
}


