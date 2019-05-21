#include "kgl_lib_pch.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"
#include "kgl_stb_image.h"

namespace kgl
{
    namespace stb
    {
        Image::Image(const char* filename, int num_components)
        {
            data_ = stbi_load(filename, &width_, &height_, &bytes_per_pixel_, num_components);
        }

        Image::Image(const std::string& filename, int num_components) :
            Image(filename.c_str(), num_components)
        {
        }

        Image::~Image()
        {
            if (data_)
                stbi_image_free(data_);
        }

        std::uint32_t Image::get_rgb(int x, int y) const
        {
            int addr = (y * width_ + x) * bytes_per_pixel_;
            std::uint32_t r = data_[addr];
            std::uint32_t g = data_[addr + 1];
            std::uint32_t b = data_[addr + 2];
            return (r << 16) + (g << 8) + b;
        }
    }
}