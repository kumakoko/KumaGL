// 天空盒对象
/*!
 * \file kgl_sky_box.h
 * \date 2017/12/31 13:57
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
#ifndef kgl_sky_box_h__
#define kgl_sky_box_h__

#include "kgl_primitive.h"
#include "kgl_cubemap_texture.h"
#include "kgl_gpu_program.h"
#include "kgl_render_state_cull_mode.h"
#include "kgl_render_state_depth.h"
#include "kgl_camera.h"
#include "kgl_basic_static_mesh.h"

namespace kgl
{
    class SkyBox
    {
    public:
        /************************************************************
        // Method:    LoadSkyBox
        // FullName:  kgl::SkyBox::LoadSkyBox
        // Access:    public 
        // Returns:   void
        // Qualifier:
        // Parameter: const std::string & positive_x_file
        // Parameter: const std::string & negative_x_file
        // Parameter: const std::string & positive_y_file
        // Parameter: const std::string & negative_y_file
        // Parameter: const std::string & positive_z_file
        // Parameter: const std::string & negative_z_file
        ************************************************************/
        void LoadSkyBox(const std::string& positive_x_file,const std::string& negative_x_file,
            const std::string& positive_y_file,const std::string& negative_y_file,
            const std::string& positive_z_file,const std::string& negative_z_file);

        /************************************************************
        // Method:    Draw
        // FullName:  kgl::SkyBox::Draw
        // Access:    public 
        // Returns:   void
        // Qualifier:
        // Parameter: CameraSPtr camera
        ************************************************************/
        void Draw();

        /************************************************************
        // Method:    ReleaseSkyBox
        // FullName:  kgl::SkyBox::ReleaseSkyBox
        // Access:    public 
        // Returns:   void
        // Qualifier:
        ************************************************************/
        void ReleaseSkyBox();

        /************************************************************
        // Method:    ~SkyBox
        // FullName:  kgl::SkyBox::~SkyBox
        // Access:    public 
        // Returns:   
        // Qualifier:
        ************************************************************/
        ~SkyBox();

        /************************************************************
        // Method:    SkyBox
        // FullName:  kgl::SkyBox::SkyBox
        // Access:    public 
        // Returns:   
        // Qualifier:
        // Parameter: CameraSPtr camera
        ************************************************************/
        SkyBox(CameraSPtr camera);
    private:
        CameraSPtr          camera_;
        CubemapTextureSPtr  cubemap_texture_;
        GPUProgramSPtr      gpu_program_;
		BasicStaticMesh*	model_;
    };
}

#endif // kgl_sky_box_h__