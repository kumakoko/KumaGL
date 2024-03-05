#include "mesh/render_data.h"
#include "kgl_texture.h"

namespace DigitalSculpt
{
    bool RenderData::ONLY_DRAW_ARRAYS = false;

    RenderData::RenderData()
    {
        _shaderType;// : opts.shader,
        _flatShading = true;// : opts.flatshading,
        _showWireframe = false;// : opts.wireframe,
        _matcap = 0;// : Math.min(opts.matcap, ShaderMatcap.matcaps.length - 1), // matcap id
        _curvature = 0;// : Math.min(opts.curvature, 5.0),

        _useDrawArrays = false;// : false,
        _vertexBuffer = new Buffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);// : new Buffer(gl, gl.ARRAY_BUFFER, gl.DYNAMIC_DRAW),
        _normalBuffer = new Buffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);// : new Buffer(gl, gl.ARRAY_BUFFER, gl.DYNAMIC_DRAW),
        _colorBuffer = new Buffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);// : new Buffer(gl, gl.ARRAY_BUFFER, gl.DYNAMIC_DRAW),
        _materialBuffer = new Buffer(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);//: new Buffer(gl, gl.ARRAY_BUFFER, gl.DYNAMIC_DRAW),
        _texCoordBuffer = new Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);// : new Buffer(gl, gl.ARRAY_BUFFER, gl.STATIC_DRAW),
        _indexBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);// : new Buffer(gl, gl.ELEMENT_ARRAY_BUFFER, gl.STATIC_DRAW),
        _wireframeBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);//: new Buffer(gl, gl.ELEMENT_ARRAY_BUFFER, gl.STATIC_DRAW),

        // these material values overrides the vertex attributes
        // it's here for debug or preview
        glm::vec3 _albedo = glm::vec3(-1.0f, -1.0f, -1.0f);// new Float32Array([-1.0, -1.0, -1.0]),
        _roughness = -0.18f;//: -0.18,
        _metallic = 0.78f;// : -0.78,
        _alpha = 1.0f;//:;//1.0,

        glm::vec3 _flatColor = glm::vec3(-1.0f, 0.0f, 0.0f);// : new Float32Array([1.0, 0.0, 0.0]),
        _mode = GL_TRIANGLES;// : gl.TRIANGLES
    };
}