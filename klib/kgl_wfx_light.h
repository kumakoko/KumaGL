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
 * \file kgl_wfx_light.h
 * \date 2019/01/27 11:21
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
#ifndef kgl_wfx_light_h__
#define kgl_wfx_light_h__

namespace kgl
{
    namespace water_effect
    {
        class Light {
        public:
            Light(const glm::vec3& position,const glm::vec3& color,const glm::vec3& attenuation = glm::vec3(1, 0, 0));

            inline const glm::vec3& getPosition() const
            { 
                return position_; 
            }
            
            inline void setPosition(const glm::vec3& position)
            {
                position_ = position; 
            }

            inline const glm::vec3& getColor() const
            {
                return color_; 
            }

            inline void setColor(const glm::vec3& color)
            {
                color_ = color;
            }

            const glm::vec3& getAttenuation() const
            {
                return attenuation_;
            }

        private:
            glm::vec3 position_;
            glm::vec3 color_;
            glm::vec3 attenuation_;
        };
    }
}

#endif // kgl_wfx_light_h__