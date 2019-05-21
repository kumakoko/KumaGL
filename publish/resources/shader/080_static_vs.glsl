#version 330

in vec3 position;
in vec2 textureCoords;
in vec3 normal;

out vec2 pass_textureCoords;
out vec3 surfaceNormal;
out vec3 toLightVector[4];
out vec3 toCameraVector;
out float visibility;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition[4];

uniform int useFakeLighting;

uniform float numberOfRows;
uniform vec2 offset;

const float density = 0.0; //0.0035;
const float gradient = 5.0;

uniform vec4 plane;

void main(void) {
    vec4 worldPosition = transformationMatrix * vec4(position, 1.0);

    gl_ClipDistance[0] = dot(worldPosition, plane);

    vec4 positionRelativeToCam = viewMatrix * worldPosition;
    gl_Position = projectionMatrix * positionRelativeToCam;
    pass_textureCoords = (textureCoords / numberOfRows) + offset;

    vec3 actualNormal = normal;
    if (useFakeLighting == 1) {
        actualNormal = vec3(0.0, 1.0, 0.0);
    }
    surfaceNormal = (transformationMatrix * vec4(actualNormal, 0.0)).xyz;
    toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
    for (int i = 0; i < 4; i++) {
        toLightVector[i] = lightPosition[i] - worldPosition.xyz;
    }

    float dist = length(positionRelativeToCam.xyz);
    visibility = exp(-pow((dist * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);
}
