// 用在水波效果上的片元着色器

#version 330 core
uniform vec4 vForce;

// 把高度值分解为RGBA四个值
vec4 EncodeHeightmap(float height)
{
	float h = height;
	
	// 如果传递进来的值是一个大于0的值，存到positive么否则存为0
	float positive = height > 0.0 ? height : 0.0;
	
	// 如果传递进来的值是一个负值，取其绝对值
	float negative = height < 0.0 ? -height : 0.0;

	vec4 color = 0;

	// 假设height为3.45，那么positive为3.45，negative为0
	color.r = positive;
	color.g = negative;
	
	// 要增加精确度时，把小数点中比较低的位数在b，a
	// color.ba存储着通过fract函数得到color.rg*256的小数部分
	color.ba = fract(color.rg*256);  // fract对应的HLSL函数名为frac
	color.rg -= color.ba /256.0;
	
	return color;
}

void main(void)
{
	gl_FragColor = EncodeHeightmap(1.0);
}
