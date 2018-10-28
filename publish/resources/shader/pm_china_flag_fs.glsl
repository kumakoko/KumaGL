#version 330 core

uniform highp vec2 screen_resolution;
uniform highp float flag_scale;
out vec4 fragment_color;

// const vec2 A = vec2(0.0f,1.0f);
// const vec2 B = vec2(-0.95106f,0.30902f);
// const vec2 C = vec2(-0.58779f,-0.80902f);
// const vec2 D = vec2(0.58779f,-0.80902f);
// const vec2 E = vec2(-0.95106f,0.30902f);
// const vec2 F = vec2(0,-0.38197f);
// const vec2 G = vec2(0.36328f,0.11804f);
// const vec2 H = vec2(0.22452f,0.30902f);
// const vec2 I = vec2(-0.22452f,0.30902f);
// const vec2 O = vec2(0.0f,0.0f);

const vec2 A = vec2(0.0,1.0);
const vec2 B = vec2(-0.95,0.31);
const vec2 C = vec2(-0.59,-0.81);
const vec2 D = vec2(0.59,-0.81);
const vec2 E = vec2(0.95,0.31);
const vec2 F = vec2(0.0,-0.38);
const vec2 G = vec2(0.36,-0.12);
const vec2 H = vec2(0.22,0.31);
const vec2 I = vec2(-0.22,0.31);
const vec2 J = vec2(-0.36,-0.11);
const vec2 O = vec2(0.0,0.0);


const float EPSILON = 0.1;
const float HALF_PI = 3.1415 / 2.0;

// 返回三个点组成三角形的面积 
float TriangleArea(vec2 a, vec2 b, vec2 c) 
{ 
    float result = abs((a.x * b.y + b.x * c.y + c.x * a.y - b.x * a.y - c.x * b.y - a.x * c.y) / 2.0f);
    return result; 
} 

// a,b,c,d是四边形四个点的顶点
// p是待判断的顶点
// 四个顶点组成的顺序是：
//  a------b
//  |      |
//  d------c
bool PointInQuadrangle(vec2 a, vec2 b, vec2 c, vec2 d, vec2 p) 
{
    // 点p和四个顶点组成的四个三角形的面积和
    float dTriangle = TriangleArea(a,b,p)+TriangleArea(b,c,p) +TriangleArea(c,d,p)+TriangleArea(d,a,p);
    float dQuadrangle = TriangleArea(a,b,c)+TriangleArea(c,d,a); 
    return abs(dTriangle-dQuadrangle) < EPSILON; 

}

vec4 RotateStar(vec4 two_star, float angle)
{
    // star.x = star.x * cos(angle) - star.y * sin(angle)
	// star.y = star.x * sin(angle) + star.y * cos(angle)
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);
	vec4 rotated_two_star = vec4(0.0);
	rotated_two_star.x = dot(two_star.xy , vec2(cos_angle,-sin_angle));
	rotated_two_star.y = dot(two_star.xy , vec2(sin_angle,cos_angle));
	rotated_two_star.z = dot(two_star.zw , vec2(cos_angle,-sin_angle));
	rotated_two_star.w = dot(two_star.zw , vec2(sin_angle,cos_angle));
	return rotated_two_star;
}

// AB xy分量是五角星顶点A的xy值，zw分量是五角星顶点B的xy值
// CD xy分量是五角星顶点C的xy值，zw分量是五角星顶点D的xy值
// EF xy分量是五角星顶点E的xy值，zw分量是五角星顶点F的xy值
// GH xy分量是五角星顶点G的xy值，zw分量是五角星顶点H的xy值
// IO xy分量是五角星顶点I的xy值，zw分量是五角星顶点O的xy值
// TSR的xy分量是水平和垂直方向上的平移度值，z分量是缩放值，w分量是旋转角度值
// p是待检测顶点的xy值
bool InStar(vec4 TSR, vec2 p)
{
    vec4 scale = vec4(TSR.z);
    vec4 translation = vec4(TSR.x,TSR.y,TSR.x,TSR.y);

    // 四边形AIOH和HEGO
    vec4 a = RotateStar(vec4(A,H) * scale,TSR.w) + translation;
    vec4 b = RotateStar(vec4(H,E) * scale,TSR.w) + translation;
    vec4 c = RotateStar(vec4(O,G) * scale,TSR.w) + translation;
    vec4 d = RotateStar(vec4(I,O) * scale,TSR.w) + translation;
    bool in_1st_quad = PointInQuadrangle(a.xy,b.xy,c.xy,d.xy,p);
    bool in_2nd_quad = PointInQuadrangle(a.zw,b.zw,c.zw,d.zw,p);
    
    // 四边形GDFO和FCJO
    a = RotateStar(vec4(G,F) * scale,TSR.w) + translation;
    b = RotateStar(vec4(D,C) * scale,TSR.w) + translation;
    c = RotateStar(vec4(F,J) * scale,TSR.w) + translation;
    d = RotateStar(vec4(O,O) * scale,TSR.w) + translation;
    bool in_3nd_quad = PointInQuadrangle(a.xy,b.xy,c.xy,d.xy,p);
    bool in_4th_quad = PointInQuadrangle(a.zw,b.zw,c.zw,d.zw,p);

    
    // 四边形JBIO
	vec2 dummy = vec2(0.0);
	a = RotateStar(vec4(J,dummy) * scale,TSR.w) + translation;
    b = RotateStar(vec4(B,dummy) * scale,TSR.w) + translation;
    c = RotateStar(vec4(I,dummy) * scale,TSR.w) + translation;
    d = RotateStar(vec4(O,dummy) * scale,TSR.w) + translation;
    bool in_5th_quad = PointInQuadrangle(a.xy,b.xy,c.xy,d.xy,p.xy);
    
    return in_1st_quad || in_2nd_quad || in_3nd_quad || in_4th_quad || in_5th_quad;
}


void main()
{
    // 把旗帜水平方向水平分为30等分，垂直方向分为20等分，大五角星的中心点，在水平方向第5格右下角，垂直方向第5格右下角，所以
    // 大五角星的translation为 screen_resolution.x / 30.0 * 5.0, screen_resolution - screen_resolution.x / 20.0 * 5.0,
    // 大五角星的外接圆半径为垂直方向3个格子，所以scale为screen_resolution.x / 20.0 * 3.0
    vec4 TSR = vec4(screen_resolution.x / 6.0,screen_resolution.y - screen_resolution.y / 4.0,
                                               screen_resolution.y / 20.0 * 3.0,0.0);
    vec2 p = vec2(gl_FragCoord.x, gl_FragCoord.y);
    bool in_big_star = InStar(TSR,p);
    
	//NewX = X * Cos(α） - Y * Sin(α)
	// NewY = X * Sin(α) + Y * Cos(α)
    
    // 第一个小五角星的translation为 screen_resolution.x / 30.0 * 10.0, screen_resolution - screen_resolution.x / 20.0 * 2.0,
    // 第一个小五角星的外接圆半径为垂直方向3个格子，所以scale为screen_resolution.x / 20.0 * 1.0
    TSR = vec4(screen_resolution.x / 30.0 * 10.0,screen_resolution.y - screen_resolution.y / 20.0 * 2.0,
                                               screen_resolution.y / 20.0, HALF_PI + atan(3.0/5.0));
    p = vec2(gl_FragCoord.x, gl_FragCoord.y);
    bool in_1st_small_star = InStar(TSR,p);
	
	// 第二个小五角星的translation为 screen_resolution.x / 30.0 * 12.0, screen_resolution - screen_resolution.x / 20.0 * 4.0,
    // 第二个小五角星的外接圆半径为垂直方向3个格子，所以scale为screen_resolution.x / 20.0 * 1.0
    TSR = vec4(screen_resolution.x / 30.0 * 12.0,screen_resolution.y - screen_resolution.y / 20.0 * 4.0,
                                               screen_resolution.y / 20.0,HALF_PI + atan(1.0/7.0));
    p = vec2(gl_FragCoord.x, gl_FragCoord.y);
    bool in_2nd_small_star = InStar(TSR,p);
	
	// 第三个小五角星的translation为 screen_resolution.x / 30.0 * 12.0, screen_resolution - screen_resolution.x / 20.0 * 7.0,
    // 第三个小五角星的外接圆半径为垂直方向3个格子，所以scale为screen_resolution.x / 20.0 * 1.0
    TSR = vec4(screen_resolution.x / 30.0 * 12.0,screen_resolution.y - screen_resolution.y / 20.0 * 7.0,
                                               screen_resolution.y / 20.0, atan(7.0/2.0));
    p = vec2(gl_FragCoord.x, gl_FragCoord.y);
    bool in_3rd_small_star = InStar(TSR,p);
	
	// 第四个小五角星的translation为 screen_resolution.x / 30.0 * 10.0, screen_resolution - screen_resolution.x / 20.0 * 9.0,
    // 第四个小五角星的外接圆半径为垂直方向3个格子，所以scale为screen_resolution.x / 20.0 * 1.0
    TSR = vec4(screen_resolution.x / 30.0 * 10.0,screen_resolution.y - screen_resolution.y / 20.0 * 9.0,
                                               screen_resolution.y / 20.0, atan(5.0/4.0));
    p = vec2(gl_FragCoord.x, gl_FragCoord.y);
    bool in_4th_small_star = InStar(TSR,p);
    
    
 
    if (in_big_star || in_1st_small_star || in_2nd_small_star || in_3rd_small_star || in_4th_small_star )
        fragment_color = vec4(1.0f,1.0f,0.0f,1.0f);
    else
        fragment_color = vec4(1.0f,0.0f,0.0f,1.0f);
}