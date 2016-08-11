// 文字渲染器
#ifndef kgl_font_renderer_h__
#define kgl_font_renderer_h__

#include "kgl_primitive.h"
#include "kgl_vertex_attribute.h"
#include "kgl_gpu_program.h"
#include "kgl_font_texture.h"
#include "kgl_font_primitive.h"

namespace kgl
{
	class FontRenderer
	{
	public:
		FontRenderer();
		~FontRenderer();
		void AddToRendered(const std::wstring& text, int32_t start_x, int32_t start_y, 
			const glm::vec4& left_top_color = glm::vec4(1.0f,1.0f,1.0f,1.0f),
			const glm::vec4& left_bottom_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
			const glm::vec4& right_top_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
			const glm::vec4& right_bottom_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			float scale = 1.0f);
		void Draw();
		void CreateFontTexture(const char* font_file_name, const char* texture_name,int32_t font_size, uint32_t texture_width, uint32_t texture_height);
		void SetCurrentFont(const std::string& texture_name);
	private:
		//************************************
		// Method:    ScreenToNDC
		// FullName:  kgl::FontRenderer::ScreenToNDC
		// Access:    private 
		// Returns:   void
		// Qualifier:
		// Parameter: int32_t x
		// Parameter: int32_t y
		// Parameter: float * ndc_x
		// Parameter: float * ndc_y
		//************************************
		void ScreenToNDC(int32_t x, int32_t y, float* ndc_x, float* ndc_y);

		//************************************
		// Method:    GetUnusedFontPrimitive
		// FullName:  kgl::FontRenderer::GetUnusedFontPrimitive
		// Access:    private 
		// Returns:   FontPrimitive*
		// Qualifier:
		// Parameter: int32_t char_count
		//************************************
		FontPrimitive* GetUnusedFontPrimitive(int32_t char_count);
	protected:
		GPUProgram*										font_shader_;
		float											view_port_width_;
		float											view_port_height_;
		std::set<FontPrimitive*, FontPrimitiveGreater>	unused_primitive_set_;
		std::vector<FontPrimitive*>						used_primitive_array_;
		std::map<std::string,FontTexture*>				font_texture_map_;	// 字体名-字体纹理映射表
		FontTexture*									current_font_texture_;
	};
}
#endif // kgl_font_renderer_h__