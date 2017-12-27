// 天空盒对象
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