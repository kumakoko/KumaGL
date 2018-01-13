/*!
 * \file font_app.h
 * \date 2017/12/31 14:49
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 演示文字渲染技术
 *
 * TODO: 演示使用kgl::FontRenderer类
 *
 * \note
*/
/**************************************************************************************************************************
Copyright(C) 2014-2016 www.xionggf.com

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
#ifndef font_app_h__
#define font_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_blend.h"

class FontApp : public kgl::App
{
public:
	FontApp();
	virtual ~FontApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame() override;
	virtual void PreRenderFrame() override;
	void InitHelper();
private:
	kgl::Primitive*			helper_rectangle_;
	kgl::RenderStateBlend	rs_blend_;
	std::wstring			text_1_;
	std::wstring			text_2_;
	std::wstring            text_3_;
	std::wstring			text_jpn_;
};

#endif // font_app_h__