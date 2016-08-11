#version 330 core

uniform vec2 screen_resolution;
out vec4 fragment_color;

void main()
{
    // 圆心位置
    vec2 circle_center = screen_resolution.xy * 0.5f;
    
    // 半径为窗口宽度的五分之一
    float radius = screen_resolution.x * 0.2f;
    
    float distance_sq = (gl_FragCoord.x - circle_center.x)*(gl_FragCoord.x - circle_center.x) + (gl_FragCoord.y - circle_center.y) * (gl_FragCoord.y - circle_center.y);
    
    if ( distance_sq < radius * radius )
        fragment_color = vec4(1.0f,0.0f,0.0f,1.0f);
    else
        fragment_color = vec4(1.0f,1.0f,1.0f,1.0f);
}