#version 330

smooth in vec2 vTexCoord;
//smooth in vec3 vNormal;

smooth in vec3 vWorldPos;
smooth in vec4 vEyeSpacePos;

out vec4 outputColor;

uniform sampler2D gSampler;
uniform vec4 vColor;

uniform vec3 vEyePosition;

//uniform Material matActive;

uniform float fAlphaTest;
uniform float fAlphaMultiplier;

void main()
{
	vec4 vTexColor = texture2D(gSampler, vTexCoord);
	float fNewAlpha = vTexColor.a*fAlphaMultiplier;               
	if(fNewAlpha < fAlphaTest)
		discard;
	
	if(fNewAlpha > 1.0f)
		fNewAlpha = 1.0f;	
		
	vec4 vMixedColor = vTexColor*vColor; 
	
	outputColor = vec4(vMixedColor.zyx, fNewAlpha);
}