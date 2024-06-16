/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
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
            srand(static_cast<unsigned int>(time(nullptr)));
            this->seed_ = rand();
            this->octaves_ = octaves;
            this->amplitude_ = amplitude;
            this->roughness_ = roughness;
        }

        float PerlinNoise::GetPerlinNoise(int x, int y)
        {
            float total = 0;
            float d = (float)pow(2.f, octaves_ - 1);

            for (int i = 0; i < octaves_; i++)
            {
                float freq = (float)(powf(2, static_cast<float>(i)) / d);
                float amp = (float)powf(roughness_, static_cast<float>(i)) * amplitude_;
                total += GetInterpolatedNoise(x * freq, y * freq) * amp;
            }
            return total;
        }

        float PerlinNoise::GetSmoothNoise(int x, int y) 
        {
            // 四个角的噪声值，四个边的噪声值，中心点的噪声
            float corners = (GetNoise(x - 1, y - 1) + GetNoise(x + 1, y - 1) + GetNoise(x - 1, y + 1)
                + GetNoise(x + 1, y + 1)) / 16.f;
            float sides = (GetNoise(x - 1, y) + GetNoise(x + 1, y) + GetNoise(x, y - 1) + GetNoise(x, y + 1)) / 8.f;
            float center = GetNoise(x, y) / 4.f;
            return corners + sides + center;
        }

        float PerlinNoise::GetNoise(int x, int y)
        {
            srand(x * 49632 + y * 325176 + seed_);
            return rand() * 2.f - 1.f;
        }

        float PerlinNoise::GetInterpolatedNoise(float x, float y)
        {
            int intX = (int)x;
            float fracX = x - intX;
            int intY = (int)y;
            float fracY = y - intY;

            float v1 = GetSmoothNoise(intX, intY);
            float v2 = GetSmoothNoise(intX + 1, intY);
            float v3 = GetSmoothNoise(intX, intY + 1);
            float v4 = GetSmoothNoise(intX + 1, intY + 1);
            float i1 = Interpolate(v1, v2, fracX);
            float i2 = Interpolate(v3, v4, fracX);
            return Interpolate(i1, i2, fracY);
        }

        float PerlinNoise::Interpolate(float a, float b, float blend)
        {
            double theta = blend * glm::pi<double>();
            float f = (float)((1.0 - cos(theta)) * 0.5f);
            return a * (1 - f) + b * f;
        }
    }
}