//
// Pixel Shader
//

// р蔼眖(r,g,b,a)4蝴秖临Θ虫计
float DecodeHeightmap(vec4 heightmap)
{
	vec4 table = vec4(1.0, -1.0, 1.0/256.0, -1.0/256.0);
	return dot(heightmap, table);
}

float DecodeHeightmap(sampler2D HeightmapSampler, vec2 texcoord)
{
	vec4 heightmap = texture2D(HeightmapSampler, texcoord);
	return DecodeHeightmap(heightmap);
}

vec4 EncodeHeightmap(float fHeight)
{
	float h = fHeight;
	float positive = fHeight > 0.0 ? fHeight : 0.0;
	float negative = fHeight < 0.0 ? -fHeight : 0.0;

	vec4 color = 0;

	// 虏虫暗猭
	color.r = positive;
	color.g = negative;
	
	// 璶糤弘絋, 碞р1计(b,a)い.
	color.ba = frac(color.rg*256);
	color.rg -= color.ba/256.0;
	
	return color;
}

uniform sampler2D HeightCurrentSampler;

uniform vec4  vTextureSize;
uniform float fNormalScale;

void main()
{
	vec2 offset[4] =
	{
		vec2(-1.0, 0.0),
		vec2( 1.0, 0.0),
		vec2( 0.0,-1.0),
		vec2( 0.0, 1.0)
	};	

	float fHeightL = DecodeHeightmap(HeightCurrentSampler, gl_TexCoord[0].xy + offset[0]*vTextureSize.xy);
	float fHeightR = DecodeHeightmap(HeightCurrentSampler, gl_TexCoord[0].xy + offset[1]*vTextureSize.xy);
	float fHeightT = DecodeHeightmap(HeightCurrentSampler, gl_TexCoord[0].xy + offset[2]*vTextureSize.xy);
	float fHeightB = DecodeHeightmap(HeightCurrentSampler, gl_TexCoord[0].xy + offset[3]*vTextureSize.xy);
	
	float3 n = vec3(fHeightB - fHeightT, fHeightR - fHeightL, fNormalScale);
	float3 normal = (n + 1.0) * 0.5;
	
	gl_FragColor = vec4(normal.rgb, 1.0);
}
