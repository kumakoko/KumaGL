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
 * \file kgl_resource_handle.h
 * \date 2019/01/26 19:54
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
#ifndef kgl_resource_handle_h__
#define kgl_resource_handle_h__

namespace kgl
{
    namespace water_effect
    {
        template <typename Deleter>
        class ResourceHandle
        {
        public:
            ResourceHandle() = default;

            ResourceHandle(GLuint id) : id(id)
            {
            }

            ResourceHandle(ResourceHandle&& other) : id_(other.id_)
            {
                other.id_ = 0;
            }

            ResourceHandle& operator=(ResourceHandle&& other)
            {
                std::swap(id_, other.id_);
                return *this;
            }

            ~ResourceHandle()
            {
                if (id_) Deleter{}(id_);
            }

            GLuint get() const
            {
                return id_;
            }

        private:
            GLuint id_ = 0;
        };


        template <class Deleter, std::size_t N = 1>
        class ResourceHandleArray
        {
        public:
            ResourceHandleArray() = default;

            ResourceHandleArray(ResourceHandleArray&& other) : ids_(std::move(other.ids_))
            {
                std::fill(std::begin(other.ids_), std::end(other.ids_), 0);
            }

            ResourceHandleArray& operator = (ResourceHandleArray&& other)
            {
                std::swap(ids_, other.ids_);
                return *this;
            }

            ~ResourceHandleArray()
            {
                Deleter()(N, ids_.data());
            }

            GLuint* operator & ()
            {
                return ids_.data();
            }

            GLuint get(std::size_t n = 0) const
            {
                return ids_[n];
            }

        private:
            std::array<GLuint, N> ids_ = {};
        };

        /// <summary>
        /// 若干个buffer的删除器
        /// </summary>
        struct GLBufferDeleter
        {
            void operator()(GLsizei count, GLuint* ptr) const;
        };

        /// <summary>
        /// 若干个frame buffer的删除器
        /// </summary>
        struct GLFramebufferDeleter
        {
            void operator()(GLsizei count, GLuint* ptr) const;
        };

        /// <summary>
        /// 若干个render buffer的删除器
        /// </summary>
        struct GLRenderbufferDeleter
        {
            void operator() (GLsizei count, GLuint* ptr) const;
        };

        /// <summary>
        /// 若干个texture的删除器
        /// </summary>
        struct GLTextureDeleter
        {
            void operator()(GLsizei count, GLuint* ptr) const;
        };

        /// <summary>
        /// 若干个vertex buffer的删除器
        /// </summary>
        struct GLVertexArrayDeleter
        {
            void operator()(GLsizei count, GLuint* ptr) const;
        };

        using BufferHandle = ResourceHandleArray<GLBufferDeleter>;
        using FramebufferHandle = ResourceHandleArray<GLFramebufferDeleter>;
        using RenderbufferHandle = ResourceHandleArray<GLRenderbufferDeleter>;
        using TextureHandle = ResourceHandleArray<GLTextureDeleter>;
        using VertexArrayHandle = ResourceHandleArray<GLVertexArrayDeleter>;
    }
}

#endif // kgl_resource_handle_h__