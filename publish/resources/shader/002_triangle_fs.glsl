in vec3 FragmentColor;
out vec4 color;

void main()
{
    // color = vec4(FragmentColor,1.0f);
    color = MakeFragVec4(FragmentColor);
}