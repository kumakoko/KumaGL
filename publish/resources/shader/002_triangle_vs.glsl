layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 FragmentColor;

void main()
{
	gl_Position = MakeVec4(position);
    // gl_Position = vec4(position.x, position.y, position.z, 1.0);
	FragmentColor = color;
}