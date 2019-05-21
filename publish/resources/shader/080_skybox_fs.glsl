#version 330

in vec3 textureCoords;
out vec4 out_Color;

uniform samplerCube cubeMap;
uniform samplerCube cubeMap2;
uniform float blendFactor;
uniform vec3 fogColor;

const float lowerLimit = 0.0f;
const float upperLimit = 30.0f;

const float levels = 10.0f;

void main(void) {
	vec4 texture1 = texture(cubeMap, textureCoords);
	vec4 texture2 = texture(cubeMap2, textureCoords);
	vec4 finalColor = mix(texture1, texture2, blendFactor);
	
	/* Cel Shading */
	//float amount = (finalColor.r + finalColor.g + finalColor.b) / 3.0f;
	//amount = floor(amount * levels) / levels;
	//finalColor.rgb = amount * fogColor;
	
	
	float factor = (textureCoords.y - lowerLimit) / (upperLimit - lowerLimit);
	factor = clamp(factor, 0.0f, 1.0f);
	out_Color = mix(vec4(fogColor, 1.0f), finalColor, factor);
}