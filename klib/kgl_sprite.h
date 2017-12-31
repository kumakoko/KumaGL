// 精灵类
/*!
 * \file kgl_sprite.h
 * \date 2017/12/31 13:58
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
#ifndef kgl_sprite_h__
#define kgl_sprite_h__

#include "kgl_gpu_program.h"
#include "kgl_primitive.h"
#include "kgl_source_texture.h"

namespace kgl
{
    class Sprite
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Sprite"/> class.
        /// </summary>
        /// <param name="shader">The shader.</param>
        Sprite(GPUProgramSPtr shader);
        
        /// <summary>
        /// Finalizes an instance of the <see cref="Sprite"/> class.
        /// </summary>
        ~Sprite();


        /// <summary>
        /// Draws the specified sprite_texture.
        /// </summary>
        /// <param name="sprite_texture">The sprite_texture.</param>
        /// <param name="sprite_position">The sprite_position.</param>
        /// <param name="sprite_size">The sprite_size.</param>
        /// <param name="sprite_rotation">The sprite_rotation.</param>
        /// <param name="sprite_color">The sprite_color.</param>
        void Draw(SourceTextureSPtr sprite_texture, glm::vec2 sprite_position, glm::vec2 sprite_size = glm::vec2(10, 10), GLfloat sprite_rotation = 0.0f, glm::vec4 sprite_color = glm::vec4(1.0f));
    private:
        // Render state
        GPUProgramSPtr shader_;
        GLuint quadVAO;
        // Initializes and configures the quad's buffer and vertex attributes
		void initRenderData();

		kgl::Primitive*  rectangle_primitive_;
	};
}
#endif // kgl_sprite_h__