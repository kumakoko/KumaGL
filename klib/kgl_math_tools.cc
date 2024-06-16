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
#include "kgl_math_tools.h"

namespace kgl
{
    const double MathTools::PERSISTENCE = 0.25;
    const int MathTools::OCTAVES_COUNT = 32;

    double MathTools::SmoothedNoise(float x, float y)
    {
        double corners = (
            Noise(static_cast<int>(x - 1.f), static_cast<int>(y - 1.f)) + 
            Noise(static_cast<int>(x + 1.f), static_cast<int>(y - 1.f)) +
            Noise(static_cast<int>(x - 1.f), static_cast<int>(y + 1.f)) + 
            Noise(static_cast<int>(x + 1.f), static_cast<int>(y + 1.f))) / 16.0;
        
        double sides = (
            Noise(static_cast<int>(x - 1.f), static_cast<int>(y)) + 
            Noise(static_cast<int>(x + 1.f), static_cast<int>(y)) + 
            Noise(static_cast<int>(x), static_cast<int>(y - 1.f)) + 
            Noise(static_cast<int>(x), static_cast<int>(y + 1.f))) / 8.0;
        
        double center = 
            Noise(static_cast<int>(x), static_cast<int>(y)) / 4.0;
        
        return corners + sides + center;
    }

    double MathTools::InterpolatedNoise(float x, float y)
    {
        int intX = static_cast<int>(x);
        double fractionalX = x - intX;
        int intY = static_cast<int>(y);
        double fractionalY = y - intY;

        double v1 = SmoothedNoise(static_cast<float>(intX), static_cast<float>(intY));
        double v2 = SmoothedNoise(static_cast<float>(intX + 1), static_cast<float>(intY));
        double v3 = SmoothedNoise(static_cast<float>(intX), static_cast<float>(intY + 1));
        double v4 = SmoothedNoise(static_cast<float>(intX + 1), static_cast<float>(intY + 1));

        double i1 = InterpolateCos(v1, v2, fractionalX);
        double i2 = InterpolateCos(v3, v4, fractionalX);

        return InterpolateCos(i1, i2, fractionalY);
    }

    double MathTools::PerlinNoise2D(float x, float y)
    {
        double  total = 0.0;

        for (int i = 0; i < MathTools::OCTAVES_COUNT; i++)
        {
            double frequency = pow(2, i);
            double amplitude = pow(MathTools::PERSISTENCE, i);
            total += InterpolatedNoise(static_cast<float>(x * frequency), static_cast<float>(y * frequency)) * amplitude;
        }

        return total;
    }

    double MathTools::Gauss(float x, float y, float offset)
    {
        float a = 3.0f;
        float e = 2.71828182846f;
        float spread = 25.0f;

        x = x + offset;
        y = y + offset;

        float term = -(powf(x, 2.0f) / (2.0f * spread) + powf(y, 2.0f) / (2.0f * spread));
        return -a * powf(e, term);
    }

    glm::mat4 MathTools::CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
    {
        glm::mat4 matrix = glm::identity<glm::mat4>();

        matrix = glm::translate(matrix, translation);
        matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        matrix = glm::scale(matrix, scale);

        return matrix;
    }

    float MathTools::BarryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos)
    {
        float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
        float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
        float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
        float l3 = 1.0f - l1 - l2;
        return l1 * p1.y + l2 * p2.y + l3 * p3.y;
    }
}