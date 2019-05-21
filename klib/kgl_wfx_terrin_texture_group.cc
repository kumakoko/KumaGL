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
#include "kgl_lib_pch.h"
#include "kgl_wfx_terrin_texture_group.h"

namespace kgl
{
    namespace water_effect
    {

        TerrainTexturePack::TerrainTexturePack(
            TerrainTexture backgroundTexture,
            TerrainTexture rTexture,
            TerrainTexture gTexture,
            TerrainTexture bTexture
            )
            : backgroundTexture_(backgroundTexture)
            , rTexture_(rTexture)
            , gTexture_(gTexture)
            , bTexture_(bTexture)
        { }

        TerrainTexture const& TerrainTexturePack::getBackgroundTexture() const {
            return backgroundTexture_;
        }

        TerrainTexture const& TerrainTexturePack::getRTexture() const {
            return rTexture_;
        }

        TerrainTexture const& TerrainTexturePack::getGTexture() const {
            return gTexture_;
        }

        TerrainTexture const& TerrainTexturePack::getBTexture() const {
            return bTexture_;
        }

    }
}