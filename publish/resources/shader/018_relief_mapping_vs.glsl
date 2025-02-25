varying vec2 v_texCoord;

uniform vec4 Tangent;
uniform vec4 Normal;
uniform vec4 BiNormal;

uniform vec4 Eye;
varying vec3 EyeDir;

void main( void )
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    v_texCoord = gl_MultiTexCoord0.xy;

    // Tangent vectors are transformed into view-space.
    // This allows the eye position to
    // be in the same space as the vertex.
    // Please note that this is not the best way of
    // making sure that everything is in the same space.
    // The best way is to inversely transform the eye by the
    // model matrix in the application and remove the next 4 
    // lines of shader (remembering to replace the transformed 
    // vectors with the original ones).

    vec3 m_Tangent  = vec3(gl_ModelViewMatrix * Tangent);
    vec3 m_Normal   = vec3(gl_ModelViewMatrix * Normal);
    vec3 m_BiNormal = vec3(gl_ModelViewMatrix * BiNormal);

    // Also, transform the vertex into view-space.
    vec3 m_Vector = vec3(gl_ModelViewMatrix * gl_Vertex);

    // Calculate the direction of the Eye Position from the vertex;
    vec3 dirEye = Eye.xyz  - m_Vector;
        
    // Put the Eye Direction into tangent space
    EyeDir.x = dot(m_Tangent.xyz , dirEye);
    EyeDir.y = dot(m_BiNormal.xyz , dirEye);
    EyeDir.z = dot(m_Normal.xyz , dirEye);
}

