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
 * \file kgl_wfx_terrin_texture_group.h
 * \date 2019/01/27 12:26
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
#ifndef kgl_wfx_terrin_texture_group_h__
#define kgl_wfx_terrin_texture_group_h__


#include "kgl_wfx_terrin_texture.h"

namespace kgl
{
    namespace water_effect
    {

        class TerrainTexturePack {
        public:
            TerrainTexturePack(
                TerrainTexture backgroundTexture,
                TerrainTexture rTexture,
                TerrainTexture gTexture,
                TerrainTexture bTexture);

            TerrainTexture const& getBackgroundTexture() const;
            TerrainTexture const& getRTexture() const;
            TerrainTexture const& getGTexture() const;
            TerrainTexture const& getBTexture() const;

        private:
            TerrainTexture backgroundTexture_;
            TerrainTexture rTexture_;
            TerrainTexture gTexture_;
            TerrainTexture bTexture_;
        };
    }
}


#endif // kgl_wfx_terrin_texture_group_h__