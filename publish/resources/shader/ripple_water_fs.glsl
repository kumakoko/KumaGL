//
// Pixel Shader
//

uniform sampler2D NormalmapSampler;
uniform sampler2D WaterSampler;

uniform float fTexcoordScale;

void main()
{
	vec4 normalmap = texture2D(NormalmapSampler, gl_TexCoord[0].xy);
	vec3 normal = (normalmap.rgb - 0.5) * 2.0;
	vec2 texcoord = gl_TexCoord[0].xy + normal.xy * fTexcoordScale;

	gl_FragColor = texture2D(WaterSampler, texcoord);
}
