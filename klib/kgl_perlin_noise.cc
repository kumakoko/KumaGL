#include "kgl_lib_pch.h"
#include "kgl_perlin_noise.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		PerlinNoise::PerlinNoise(int seed, int octaves, float amplitude, float roughness) 
		{
			this->seed_ = seed;
			this->octaves_ = octaves;
			this->amplitude_ = amplitude;
			this->roughness_ = roughness;
		}

		PerlinNoise::PerlinNoise(int octaves, float amplitude, float roughness)
		{
			srand(time(nullptr));
			this->seed_ = rand();
			this->octaves_ = octaves;
			this->amplitude_ = amplitude;
			this->roughness_ = roughness;
		}

		float PerlinNoise::getPerlinNoise(int x, int y)
		{
			float total = 0;
			float d = (float)pow(2.f, octaves_ - 1);

			for (int i = 0; i < octaves_; i++)
			{
				float freq = (float)(powf(2, i) / d);
				float amp = (float)powf(roughness_, i) * amplitude_;
				total += getInterpolatedNoise(x * freq, y * freq) * amp;
			}
			return total;
		}

		float PerlinNoise::getSmoothNoise(int x, int y) 
		{
			// 四个角的噪声值，四个边的噪声值，中心点的噪声�?
			float corners = (getNoise(x - 1, y - 1) + getNoise(x + 1, y - 1) + getNoise(x - 1, y + 1)
				+ getNoise(x + 1, y + 1)) / 16.f;
			float sides = (getNoise(x - 1, y) + getNoise(x + 1, y) + getNoise(x, y - 1) + getNoise(x, y + 1)) / 8.f;
			float center = getNoise(x, y) / 4.f;
			return corners + sides + center;
		}

		float PerlinNoise::getNoise(int x, int y)
		{
			srand(x * 49632 + y * 325176 + seed_);
			return rand() * 2.f - 1.f;
		}

		float PerlinNoise::getInterpolatedNoise(float x, float y)
		{
			int intX = (int)x;
			float fracX = x - intX;
			int intY = (int)y;
			float fracY = y - intY;

			float v1 = getSmoothNoise(intX, intY);
			float v2 = getSmoothNoise(intX + 1, intY);
			float v3 = getSmoothNoise(intX, intY + 1);
			float v4 = getSmoothNoise(intX + 1, intY + 1);
			float i1 = interpolate(v1, v2, fracX);
			float i2 = interpolate(v3, v4, fracX);
			return interpolate(i1, i2, fracY);
		}

		float PerlinNoise::interpolate(float a, float b, float blend)
		{
			double theta = blend * glm::pi<double>();
			float f = (float)((1.0 - cos(theta)) * 0.5f);
			return a * (1 - f) + b * f;
		}
	}
}