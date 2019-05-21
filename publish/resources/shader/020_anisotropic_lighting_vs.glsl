uniform vec4 eyePosition;
uniform vec4 lightVector;

void main( void )
{
	// Transform the normal vector using only the rotational portion of the 
	// full inverse model-view matrix.

    vec3 tmpNorm = gl_NormalMatrix * gl_Normal;
    vec4 worldNormal;
    worldNormal.x = tmpNorm.x;
    worldNormal.y = tmpNorm.y;
    worldNormal.z = tmpNorm.z;
    worldNormal.w = 0.0;

	// Compute world-space position
	vec4 worldSpacePos = gl_ModelViewMatrix * gl_Vertex;

	// Create a normalized vector pointing from vertex to eye
	vec4 vertToEye = normalize( eyePosition - worldSpacePos );

	// h = normalize(l + e)
	vec4 halfAngle = normalize( vertToEye + lightVector );

	//
	// This is where we use our texture as a look up table for anisotropic 
	// lighting.
	//

    gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[0].x = max( dot(lightVector, worldNormal), 0.0);
	gl_TexCoord[0].y = max( dot(halfAngle, worldNormal), 0.5);

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    gl_FrontColor = gl_Color;
}
