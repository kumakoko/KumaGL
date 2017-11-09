// 纹理类的基类
#include "kgl_lib_pch.h"
#include "kgl_cubemap_texture.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_defines.h"

#if defined(__APPLE__) && defined(__MACH__)
#include "SOIL.h"
#endif

namespace kgl
{
    static const GLenum types[6] =
    {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    CubemapTexture::CubemapTexture(
        const std::string& positive_x_file,
        const std::string& negative_x_file,
        const std::string& positive_y_file,
        const std::string& negative_y_file,
        const std::string& positive_z_file,
        const std::string& negative_z_file)
    {
        file_name_[0] = positive_x_file;
        file_name_[1] = negative_x_file;
        file_name_[2] = positive_y_file;
        file_name_[3] = negative_y_file;
        file_name_[4] = positive_z_file;
        file_name_[5] = negative_z_file;
        texture_id_ = 0;
    }

    CubemapTexture::~CubemapTexture()
    {
        if (texture_id_ != 0) 
        {
            glDeleteTextures(1, &texture_id_);
        }
    }

    bool CubemapTexture::Load()
    {
        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);

        int image_width = 0;
        int image_height = 0;
        unsigned char* image = nullptr;

        // 装载并生成6个纹理
        for (unsigned int i = 0; i < 6; i++)
        {
            image_width = 0;
            image_height = 0;
            image = SOIL_load_image(file_name_[i].c_str(),&image_width, &image_height, 0, SOIL_LOAD_RGB);

            if (nullptr == image)
            {
                std::wstringstream wss;
                wss << L"Can not load image file from " << StringConvertor::ANSItoUTF16LE(file_name_[i].c_str());
                throw Error(wss.str(), __FILE__, __LINE__);
            }

            // 注意这个参数，依次创建立方体纹理的六个面对应的纹理
            glTexImage2D(types[i], 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            SOIL_free_image_data(image);
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // 创建完毕之后要解绑定
        return true;
    }

    void CubemapTexture::ActiveBind(GLuint slot_index)
    {
        GLuint i = glm::clamp(slot_index, static_cast<GLuint>(0), static_cast<GLuint>(8));
        GLenum texture_unit = GL_TEXTURE0 + i;
        glActiveTexture(texture_unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
    }
}
