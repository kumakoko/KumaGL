/*************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute,sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,OUT OF OR IN CONNECTION WITH THE 
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
************************************************************************************/
/*
��һ���ı����ϻ���ˮ��Ч��
*/
#version 330

in vec2 position;
out vec4 clipSpace;            // ���㾭���任���ڲü��ռ��е�λ������
out vec2 textureCoords;
out vec3 toCameraVector;       // �����㵽��������������� 
out vec3 fromLightVector;      // ��Դ�����������������

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;
const float tiling = 4.0;

void main(void) 
{
    vec4 worldPosition = modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
    clipSpace = projectionMatrix * viewMatrix * worldPosition;
    gl_Position = clipSpace;
    textureCoords = (position.xy / 2.0 + 0.5) * tiling;    // ʹ��ƽ�̵ķ�ʽ����ˮ�����������ʶ�Ҫ����tiling
    toCameraVector = cameraPosition - worldPosition.xyz;   // �����㵽��������������� 
    fromLightVector = worldPosition.xyz - lightPosition;   // ��Դ�����������������
}
