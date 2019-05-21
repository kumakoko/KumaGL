#version 330

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;
out vec4 out_Color;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform vec3 lightColor;
uniform float moveFactor;
const float waveStrength = 0.04;
const float shineDamper = 20.0;
const float reflectivity = 0.5;
const float near = 0.1;
const float far = 1000.0;

void main(void)
{
	// 得到当前片元在NDC空间中的坐标
    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;


    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);

	// 深度纹理
    float depth = texture(depthMap, refractTexCoords).r;
    
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
    
	depth = gl_FragCoord.z; // 拿到本片元的z值


    float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
    float waterDepth = floorDistance - waterDistance;
    vec2 distortedTexCoords = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 0.1;
    distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
    vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth / 20.0, 0.0, 1.0);
    reflectTexCoords.x = clamp(reflectTexCoords.x + totalDistortion.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y + totalDistortion.y, -0.999, -0.001);
    refractTexCoords = clamp(refractTexCoords + totalDistortion, 0.001, 0.999);
    vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColor = texture(refractionTexture, refractTexCoords);
    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 normal = normalize(vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0));
    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, normal);
    refractiveFactor = pow(refractiveFactor, 0.5);
    refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);
    vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = lightColor * specular * reflectivity * clamp(waterDepth / 5.0, 0.0, 1.0);
    out_Color = mix(reflectColor, refractColor, refractiveFactor);
    out_Color = mix(out_Color, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specularHighlights, 0.0);
    out_Color.a = clamp(waterDepth / 5.0, 0.0, 1.0);
}

