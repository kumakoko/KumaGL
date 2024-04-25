//import getOptionsURL from 'misc/getOptionsURL';
//import Buffer from 'render/Buffer';
//import ShaderMatcap from 'render/shaders/ShaderMatcap';
#ifndef digital_sculpt_render_data_h__
#define digital_sculpt_render_data_h__

#include "render/buffer.h"
#include "kgl_texture.h"

namespace DigitalSculpt
{
    class Mesh;

    class RenderData
    {
    public:
        static bool ONLY_DRAW_ARRAYS;
        int _shaderType;// : opts.shader,
        bool _flatShading;// : opts.flatshading,
        bool _showWireframe;// : opts.wireframe,
        int _matcap;// : Math.min(opts.matcap, ShaderMatcap.matcaps.length - 1), // matcap id
        int _curvature;// : Math.min(opts.curvature, 5.0),
        kgl::TextureSPtr _texture0;//: null,

        bool _useDrawArrays;// : false,
        BufferSPtr _vertexBuffer;// : new Buffer(gl, gl.ARRAY_BUFFER, gl.DYNAMIC_DRAW),
        BufferSPtr _normalBuffer;// : new Buffer(gl, gl.ARRAY_BUFFER, gl.DYNAMIC_DRAW),
        BufferSPtr _colorBuffer;// : new Buffer(gl, gl.ARRAY_BUFFER, gl.DYNAMIC_DRAW),
        BufferSPtr _materialBuffer;//: new Buffer(gl, gl.ARRAY_BUFFER, gl.DYNAMIC_DRAW),
        BufferSPtr _texCoordBuffer;// : new Buffer(gl, gl.ARRAY_BUFFER, gl.STATIC_DRAW),
        BufferSPtr _indexBuffer;// : new Buffer(gl, gl.ELEMENT_ARRAY_BUFFER, gl.STATIC_DRAW),
        BufferSPtr _wireframeBuffer;//: new Buffer(gl, gl.ELEMENT_ARRAY_BUFFER, gl.STATIC_DRAW),

        // these material values overrides the vertex attributes
        // it's here for debug or preview
        glm::vec3 _albedo;// new Float32Array([-1.0, -1.0, -1.0]),
        float _roughness;//: -0.18,
        float _metallic;// : -0.78,
        float _alpha;//:;//1.0,

        glm::vec3 _flatColor;// : new Float32Array([1.0, 0.0, 0.0]),
        GLenum _mode;// : gl.TRIANGLES

    public:
        RenderData(Mesh* mesh);
    };
}

#endif // digital_sculpt_render_data_h__
