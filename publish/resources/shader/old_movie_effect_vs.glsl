// 展示旧电影效果的shader，顶点着色器
#version 330 core
layout (location = 0) in vec3 position;    // 输入到顶点着色器的顶点position
layout (location = 1) in vec3 tex_coord_0; // 输入到顶点着色器的顶点第一层纹理坐标

out vec4 scene_coord;           // 银幕坐标，存放在0号纹理坐标位置
out vec4 dust_0_1_coords; 
out vec4 dust_2_3_coords; 
out vec4 tv_coords;
out vec4 noise_coords;
out vec4 line_0_vert_coords;
out vec4 line_1_vert_coords;
out vec4 line_2_vert_coords;
out vec4 overexposure;

uniform vec2  window_size; // 当前屏幕的大小
uniform float current_time; // 当前的时间值，从外部传入到vs中
uniform float max_frame_jitter; // 每一帧的抖动范围值，可调配，最小0，最大10，默认1.4，每调整一次，数值变化值为0.1
uniform float frame_jitter; // 每一帧的抖动频率值，可调配，最小0，最大6，默认4.7，每调整一次，数值变化值为0.1
uniform float over_exposure_amount; // 曝光过度效果的数量值，可调配，最小0，最大10，默认1.5，每调整一次，数值变化值为0.1
uniform float grain_thickness; // 木纹效果的数量值，可调配，最小0.1，最大4，默认1，每调整一次，数值变化值为0.1
uniform float dust_amount; // 灰尘的数量值，可调配，最小0.0，最大4.0，默认4，每调整一次，数值变化值为1
uniform float scratches_amount; // 刮痕数量

void main()
{
    // 利用当前传递进来的当时时间值，算出6个伪随机数 
    float random_0 = mod(current_time , 7.000);
    float random_1 = mod(current_time , 3.300);
    float random_2 = mod(current_time , 0.910);
    float random_3 = mod(current_time , 0.110);
    float random_4 = mod(current_time , 0.070);
    float random_5 = mod(current_time , 0.013);

    // 直接使用传递进来的坐标，传递进顶点着色器的坐标，其实就已经是一个
    // 基于NDC的规格化坐标来的
    gl_Position = vec4(position, 1);

    vec2 inverseWindowSize = vec2(1.0, 1.0) / window_size;

    // 计算在每一帧中，垂直方向上，帧抖动的范围值
    float frame_jitter_y =  40 * max_frame_jitter * random_2 * random_0 * random_3;
    
    if (frame_jitter_y < (6 - frame_jitter) * 10)
        frame_jitter_y = 0;
        
    frame_jitter_y *=inverseWindowSize.y; // 垂直方向上的抖动值除以屏幕的高，等到一个基于NDC坐标的抖动坐标值

    // 顶点的原始的纹理坐标，加上垂直方向上的抖动值，得到将要输出到片元着色器的纹理映射坐标值
    scene_coord =  vec4(tex_coord_0.x,tex_coord_0.y,tex_coord_0.z, 1.0) + vec4(0, frame_jitter_y, 0,0);

    // 计算曝光过度（overexposure）的数值compute overexposure amount
    overexposure.x = over_exposure_amount * random_3;
    
    // 传递未经计算的顶点原始坐标，用来渲染边界使用
    tv_coords = vec4(tex_coord_0, 1);

    // 计算噪声坐标
    vec2 tmp_noise_coords = (window_size / (grain_thickness * vec2(128.0, 128.0))) * tex_coord_0.xy;
    tmp_noise_coords += vec2(100 * random_3 * random_1 - random_0, random_4 + random_1 * random_2);
    noise_coords = tmp_noise_coords.xyxy;

    // 根据灰尘的数量去算出对应的纹理坐标
    if (dust_amount > 0)
    {
        dust_0_1_coords.xy = 2.0 * position.xy + 200 * vec2(random_1 * random_4, mod(current_time,0.03));
    }
    else
    {
        dust_0_1_coords.xy = vec2(0.0,0.0);
    }

    if (dust_amount > 1)
    {
        dust_0_1_coords.zw = 2.3 * position.yx - 210 * vec2(random_4 * 0.45, random_5 * 2);
    }
    else
    {
        dust_0_1_coords.zw = vec2(0.0,0.0);
    }

    if (dust_amount > 2)
    {
        dust_2_3_coords.xy = 1.4 * position.xy + vec2(700, +100) * vec2(random_2 * random_4, random_2);
    }
    else
    {
        dust_2_3_coords.xy = vec2(0.0,0.0);
    }

    if (dust_amount > 3)
    {
        dust_2_3_coords.zw = 1.7 * position.yx  + vec2(-100, 130) * vec2(random_2 * random_4, random_1 * random_4);
    }
    else
    {
        dust_2_3_coords.zw = vec2(0.0,0.0);
    }
    
    // 计算扫描线的纹理坐标
    line_0_vert_coords   = 0.5 * position.xxxx * window_size.xxxx * 0.3;
    line_1_vert_coords = line_0_vert_coords;
    line_2_vert_coords = line_0_vert_coords;

    // 第1条扫描线
    if (scratches_amount > 0.0)
    {
        line_0_vert_coords.x += 0.15 - ((random_1 - random_3 + random_2) * 0.1) * window_size.x;
    }
    else
    {
        line_0_vert_coords.x = -1;
    }

    // 第2条扫描线
    if (scratches_amount > 1.0)
    {
        line_1_vert_coords.x += 0.55 + ((random_0 - random_2 + random_4) * 0.1) * window_size.x;
    }
    else
    {
        line_1_vert_coords.x = -1.0;
    }

    // 第3条扫描线
    if (scratches_amount > 2.0)
    {
        line_2_vert_coords.x += 0.31 + ((random_1 - random_2 + random_4) * 0.2) * window_size.x;
    }
    else
    {
        line_2_vert_coords.x = -1.0;
    }
}