// 精灵类
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