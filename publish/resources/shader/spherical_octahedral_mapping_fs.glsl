// Spherical / Octahedral mapping
// Copyright (c) 2022 by Denis Reischl
//
// SPDX-License-Identifier: MIT

/*
    Sphereical / Octahedral mapping
    
    Another approach to spherical mapping. Half spherical, other
    half octahedral mapped.
    
    I'm trying to convert octahedral side from hexagonal to cartesian 
    to provide the (2D) UV map.
    
    Use Mouse to rotate/zoom.
    
*/

// 太阳
#version 330 core

// gl_FragCoord是个vec4，四个分量分别对应x, y, z和1/w。其中，x和y是当前片元的窗口相对坐标，
// 不过它们不是整数，小数部分恒为0.5。x - 0.5和y - 0.5分别位于[0, windowWidth - 1]和
// [0, windowHeight - 1]内。windowWidth和windowHeight都以像素为单位，
// 亦即用glViewPort指定的宽高。w即为乘过了投影矩阵之后点坐标的w，
// 用于perspective divide的那个值。gl_FragCoord.z / gl_FragCoord.w可以得到当前片元和camera之间的距离


uniform sampler2D texture_channel_1;
uniform vec2 screen_resolution;
uniform float global_time;
uniform vec3 iMouse;
out vec4 cOut;


#define PI 3.141592654f

#define NOISE_BLEND
// #define NORMAL_COLOR

// intersection
struct Intersection
{
    // hit entry, exit (T + position)
    vec4 vTen, vTex;
    // normal entry, exit
    vec3 vNen, vNex;
};

/// Hex to cartesian coordinates
vec2 HexXY(vec2 vUv)
{
	// get cartesian coords
	return vec2( (vUv.x * 3.f - vUv.y * 1.5f) / sqrt(3.f), vUv.y * 1.5f );
}

// spherical mapping by sphere normal...
// from http://www.raytracerchallenge.com/bonus/texture-mapping.html
vec2 spherical_map(vec3 vNor)
{
    // compute the azimuthal angle : -π < theta <= π
    // angle increases clockwise as viewed from above,
    // which is opposite of what we want, but we'll fix it later.
    float fTheta = atan(vNor.x, vNor.z);

    // compute the polar angle : 0 <= phi <= π
    float fPhi = acos(vNor.y);

    // -0.5 < raw_u <= 0.5
    float fRaw_U = fTheta / (2.f * PI);

    // 0 <= u < 1
    // here's also where we fix the direction of u. Subtract it from 1,
    // so that it increases counterclockwise as viewed from above.
    float fU = 1.f - (fRaw_U + .5f);

    // we want v to be 0 at the south pole of the sphere,
    // and 1 at the north pole, so we have to "flip it over"
    // by subtracting it from 1.
    float fV = 1.f - fPhi / PI;

    return vec2(fU, fV);
}

// octahedral mapping by sphere normal...
vec2 octahedral_map(vec3 vNor)
{
    // octahedral map
    return HexXY(vec2( max(max(
                    abs(abs(vNor.x) - abs(vNor.z)) , 
                    abs(abs(vNor.y) - abs(vNor.x)) ), 
                    abs(abs(vNor.z) - abs(vNor.y)) ),
                 min(min(
                    abs(abs(vNor.x) - abs(vNor.z)) , 
                    abs(abs(vNor.y) - abs(vNor.x)) ), 
                    abs(abs(vNor.z) - abs(vNor.y)) )));
}

// sphere intersection... based on work from iq : https://iquilezles.org/articles/intersectors/
Intersection iSphere(in vec3 vOri, in vec3 vDir, in vec3 vCen, float fRad)
{
    // get local origin
    vec3 vOriL = vOri - vCen;
    
    // ortho project local origin->direction
    float fOD = dot(vOriL, vDir);
    
    // square distance origin->center minus radius
    float fOR = dot(vOriL, vOriL) - fRad*fRad;
    
    // square hit center (!)
    float fTHitS = fOD*fOD - fOR;
    
    // no intersection
    Intersection sRet = Intersection( vec4(-1.f), vec4(-1.f), vec3(0.f), vec3(0.f));
    if( fTHitS < .0f ) return sRet;
    
    // hit distance to center disk
    fTHitS = sqrt(fTHitS);
    
    // hit vector + position
    sRet.vTen.x = -fOD - fTHitS;
    sRet.vTex.x = -fOD + fTHitS;
    sRet.vTen.yzw = vOri + vDir * sRet.vTen.x;
    sRet.vTex.yzw = vOri + vDir * sRet.vTex.x;
    
    // normals
    sRet.vNen = normalize(sRet.vTen.yzw - vCen);
    sRet.vNex = normalize(sRet.vTex.yzw - vCen);

    // return with normals
    return sRet;
}

// lookat matrix.. from https://www.shadertoy.com/view/Xtl3W2
mat3 LookAt(in vec3 vOri, in vec3 vTar, in float fRoll )
{
    vec3 vW = normalize( vTar - vOri );
    vec3 vU = normalize( cross(vW, vec3(sin(fRoll),cos(fRoll),0.0) ) );
    vec3 vV = normalize( cross(vU,vW));
    return mat3( vU, vV, vW );
}

// simple checkers
float checkers_001(vec2 vUv, float fDist)
{
    return mix(0.3, smoothstep(0.005, 0.005 + fDist * 0.005, min(fract(vUv.x), fract(vUv.y))) * (max(mod(floor(vUv.x), 2.), mod(floor(vUv.y), 2.)) * .25 + .75),
        (1. - smoothstep(0.995 - fDist * 0.005, 0.995, max(fract(vUv.x), fract(vUv.y)))));
}

// simplex noise by iq : https://www.shadertoy.com/view/Msf3WH
vec2 hash( vec2 p )
{
	p = vec2( dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)) );
	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}
float noise( in vec2 p )
{
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;

	vec2  i = floor( p + (p.x+p.y)*K1 );
    vec2  a = p - i + (i.x+i.y)*K2;
    float m = step(a.y,a.x); 
    vec2  o = vec2(m,1.0-m);
    vec2  b = a - o + K2;
	vec2  c = a - 1.0 + 2.0*K2;
    vec3  h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
	vec3  n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
    return dot( n, vec3(70.0) );
}

void main( /*in vec2 vUv*/ )
{
	// create view ray
    vec2 vP = (-screen_resolution.xy + 2.0 * gl_FragCoord.xy) / screen_resolution.y;
    vec2 vM = (iMouse.z > 0.f) ? abs(iMouse.xy / screen_resolution.xy) * vec2(8.f, 3.f) + 1.f : vec2(global_time, 2.f);
    vec3 vOri = vec3(sin(vM.x) * vM.y, 2.f, cos(vM.x) * vM.y);
	vec3 vDir = normalize(LookAt(vOri, vec3(0.f), 0.f) * vec3(vP.xy, 2.f) );
    
    // get sphere
    Intersection sInt = iSphere(vOri, vDir, vec3(0.f), 1.f);
    vec3 vPos = sInt.vTen.yzw;
    vec3 vNor = sInt.vNen;
    float fTHit = sInt.vTen.x;
    if (fTHit > 0.f)
    {       
        // spherical map
        vec2 vSUv = spherical_map(vNor);
        vec4 cOutS = mix(vec4(.2f, .2f, .2f, 1.f), vec4(1.f), checkers_001(vSUv * 16.f, fTHit * 3.f));
        
        // octahedral map
        vec2 vOUv = octahedral_map(vNor);
        vec4 cOutH = mix(vec4(.2f, .2f, .2f, 1.f), vec4(1.f), checkers_001(vOUv * 6.f, fTHit * 9.f));
        
        // half sphere each mapping
        cOut = (vNor.x > 0.f) ? cOutS : cOutH;
        
        #ifdef NOISE_BLEND
        // blend with noise by time
        float fNoise = ((vNor.x > 0.f) ? noise(vSUv * 32.f) : noise(vOUv * 16.f)) * .5f + .5f;
        cOut.xyz = mix(cOut.xyz, vec3(fNoise), (sin(global_time) + 1.f) * .5f);
        #endif
        
        #ifdef NORMAL_COLOR
        // set normal color ?
        vec3 vNorRGB = vec3(
            (vNor.x > 0.f) ? 1.f : 0.f,
            (vNor.y > 0.f) ? 1.f : 0.f,
            (vNor.z > 0.f) ? 1.f : 0.f);
        cOut.xyz = mix(cOut.xyz, vNorRGB, .2f);
        #endif
        
        // simple diffuse
        cOut.xyz *= max(dot(normalize(vec3(-4.f, 5.f, -6.f)), vNor), 0.3f);
    }
    else
        cOut = vec4(abs(vDir.y) + (sin(vDir.x * PI * 16.f) + cos(vDir.z * PI * 16.f)) * .01f);
}