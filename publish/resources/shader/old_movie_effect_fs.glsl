// 展示旧电影效果的shader，片元着色器
#version 330 core

in vec4 scene_coord;            // 银幕坐标，存放在0号纹理坐标位置
in vec4 dust_0_1_coords;    // 0号和1号灰尘坐标，存放在1号纹理坐标位置
in vec4 dust_2_3_coords;    // 2号和3号灰尘坐标，存放在2号纹理坐标位置
in vec4 tv_coords;
in vec4 noise_coords;
in vec4 line_0_vert_coords;
in vec4 line_1_vert_coords;
in vec4 line_2_vert_coords;
in vec4 overexposure;

uniform sampler2D texture_scene;
uniform sampler2D texture_dust;
uniform sampler2D texture_line;
uniform sampler2D texture_tv;
uniform sampler2D texture_noise;
uniform float grain_amount;     // 木纹效果的数量值，可调配，最小0，最大1，默认0.8，每调整一次，数值变化值为0.1
uniform float scratches_level;  //刮痕级别
uniform vec4 film_color; // 电影的整体色调

out vec4 fragment_color;

void main()
{   
    // 取出原始的画面图
    vec4 img = texture(texture_scene, scene_coord.xy);

    // 墨化画面图颜色
    vec4 img2 = dot(img, vec4(0.30, 0.59, 0.11, 0.3)) * film_color;

    // 对灰尘纹理进行采样
    vec4 dust0 = texture(texture_dust, dust_0_1_coords.xy);
    vec4 dust1 = texture(texture_dust, dust_0_1_coords.wz);
    vec4 dust2 = texture(texture_dust, dust_2_3_coords.xy);
    vec4 dust3 = texture(texture_dust, dust_2_3_coords.wz);

    // 对扫描线纹理进行采样
    vec4 line0 = texture(texture_dust, line_0_vert_coords.xy);
    vec4 line1 = texture(texture_dust, line_1_vert_coords.xy);
    vec4 line2 = texture(texture_dust, line_2_vert_coords.xy);

    // 绘制银幕四周的边框
    vec4 tv = texture(texture_tv, tv_coords.xy);

    // 对噪声纹理进行采样
    // GLSL的smoothstep，等同于HLSL的lerp
    vec4 noise_value = texture(texture_noise, noise_coords.xy);
    noise_value = smoothstep(vec4(1.0, 1.0, 1.0, 1.0), noise_value, vec4(grain_amount,grain_amount,grain_amount,grain_amount));

    // 累积灰尘的值
    float final_dust = dust0.x * dust1.y * dust2.z * dust3.w;

    // 累积线的颜色
    float final_lines = line0.x * line1.x * line2.x;
    final_lines = smoothstep(1.0, final_lines, scratches_level);
    
    // 最终合成的颜色
    fragment_color = tv * (overexposure + final_dust * final_lines * img2) * 0.888; // noise_value.wwww;
}