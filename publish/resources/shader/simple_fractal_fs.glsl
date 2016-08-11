#version 330 core

uniform vec2 constant;
uniform vec2 center;
uniform float  scale;
uniform int    maxDepth = 100;
uniform vec4 imageColor;
uniform float  colorPower;
uniform sampler2D texture_channel_1;

in vec2 texture_coord_1;
out vec4 fragment_color;

// 复数乘法
vec2 complex_mul(vec2 a , vec2 b)
{
	vec2 result;
	result.x = a.x * b.x - a.y * b.y;
	result.y = a.x * b.y + a.y * b.x;
	return result;
}

// 复数乘方
vec2 complex_pow(vec2 a , int ex)
{
	vec2 result = a;

	for(int i = 0 ; i < ex - 1; i ++)
    {
		result = complex_mul(result , a );
	}

    return result;
}

bool escap(vec2 z)
{
    return z.x > max(2.0f , length(constant)); 
}

vec2 func(vec2 z )
{
     vec2 _ret = complex_pow(z , 2 ) + constant;
     return _ret;
}

void main()
{
	vec2 z = (texture_coord_1.xy - center ) * scale;
	vec2 texNewCoord = vec2(0.9f , 0.5); 
	
	for(int i = 0 ; i < maxDepth ; i ++)
	{
		z = func(z);

		if(escap(z))
		{
			texNewCoord = vec2( (i + 0.2f) /maxDepth , 0.5);
			i = maxDepth;
		}
	}

   fragment_color = colorPower * imageColor * texture( texture_channel_1, texNewCoord );
}