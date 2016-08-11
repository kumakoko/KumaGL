// 纹理类的基类
#include "kgl_lib_pch.h"
#include "kgl_texture.h"

namespace kgl
{
	Texture::Texture() :type_(NONE)
	{

	}

	Texture::~Texture()
	{

	}

	void Texture::CreateFromFile(const std::string& file_name, const TextureParams& texture_params)
	{

	}

	void Texture::Create(int width, int height, const TextureParams& texture_params)
	{

	}

	int Texture::GetWidth() const
	{
		return 0;
	}

	int Texture::GetHeight() const
	{
		return 0;
	}

	bool Texture::IsMipmap() const
	{
		return false;
	}

	void Texture::ActiveBind(GLuint slot_index)
	{

	}
}