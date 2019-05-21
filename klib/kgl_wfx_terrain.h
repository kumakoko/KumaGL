/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

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
/*!
 * \file kgl_wfx_terrain.h
 * \date 2019/01/27 12:15
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef kgl_wfx_terrain_h__
#define kgl_wfx_terrain_h__

#include "kgl_wfx_terrin_texture.h"
#include "kgl_wfx_terrin_texture_group.h"
#include "kgl_wfx_raw_model.h"
#include "kgl_wfx_loader.h"
#include "kgl_stb_image.h"

namespace kgl
{
    namespace water_effect
    {
        class Terrain
        {
        public:
            Terrain(GLfloat x, GLfloat z, LoaderSPtr loader, TerrainTexturePack texturePack,TerrainTexture blendMap, const std::string& heightMap);

            inline GLfloat getX() const
            {
                return x_; 
            }
            
            inline GLfloat getZ() const 
            {
                return z_;
            }
            
            inline const RawModel& getRawModel() const 
            {
                return model_;
            }
            
            inline const TerrainTexturePack& getTexturePack() const 
            {
                return texturePack_;
            }
            
            inline const TerrainTexture& Terrain::getBlendMap() const 
            { 
                return blendMap_;
            }

            //************************************
            // Method:    getHeightOfTerrain
            // FullName:  kgl::water_effect::Terrain::getHeightOfTerrain
            // Access:    public 
            // Returns:   float
            // Qualifier: const
            // Parameter: float worldX
            // Parameter: float worldZ
            //************************************
            float getHeightOfTerrain(float worldX, float worldZ) const;
        private:
            RawModel generateTerrain(LoaderSPtr loader, const std::string& heightMap);
            glm::vec3 calculateNormal(unsigned int x, unsigned int y, const stb::Image& image);
            GLfloat getHeight(unsigned int x, unsigned int z, const stb::Image& image);
        private:
            std::vector<std::vector<float>> heights_;
            LoaderSPtr                      loader_;
            GLfloat                         x_;
            GLfloat                         z_;
            RawModel                        model_;
            TerrainTexturePack              texturePack_;
            TerrainTexture                  blendMap_;
        };

    }
}

#endif // kgl_wfx_terrain_h__