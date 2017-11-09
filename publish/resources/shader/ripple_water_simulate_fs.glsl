#version 330 core

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

	color.r = positive;
	color.g = negative;
	
	color.ba = frac(color.rg*256);
	color.rg -= color.ba/256.0;
	
	return color;
}

uniform sampler2D HeightPrevSampler;
uniform sampler2D HeightCurrentSampler;

uniform vec4  vTextureSize;
uniform float fDamping;

void main()
{
	vec3 offset[4] =
	{
		vec3(-1.0, 0.0, 0.25),
		vec3( 1.0, 0.0, 0.25),
		vec3( 0.0,-1.0, 0.25),
		vec3( 0.0, 1.0, 0.25),
	};	

	float fHeightPrev = DecodeHeightmap(HeightPrevSampler, gl_TexCoord[0].xy);
	float fNeighCurrent = 0;
	
	for ( int i=0; i<4; i++ )
	{
		vec2 vTexcoord = gl_TexCoord[0].xy + offset[i].xy * vTextureSize.xy;
		fNeighCurrent += (DecodeHeightmap(HeightCurrentSampler, vTexcoord) * offset[i].z);
	}	
	
	float fHeight = fNeighCurrent * 2.0 - fHeightPrev;
	fHeight *= fDamping;
	gl_FragColor = EncodeHeightmap(fHeight);
}
