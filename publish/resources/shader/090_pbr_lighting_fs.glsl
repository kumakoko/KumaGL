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
// 演示在各个坐标空间中进行变换的示例程序的fragment shader
#version 330 core

out vec4 fragment_color;
in vec2 fragment_tex_coord_1;
in vec3 fragment_world_pos;
in vec3 fragment_normal;

// material parameters
uniform sampler2D u_albedo_map; // 反照率贴图
uniform sampler2D u_normal_map; // 法线贴图
uniform sampler2D u_metallic_map; // 金属度贴图
uniform sampler2D u_roughness_map; // 粗糙度贴图
uniform sampler2D u_ambient_occlusion_map; // 环境遮蔽贴图

// 基础反射率，用来调整非金属材质的基础反射率，对于金属材质，基础反射率直接用其
// 反照率颜色控制便可
uniform vec3 u_base_reflectivity;
uniform vec3 u_light_pos_in_world; // 灯光在世界空间中的位置坐标
uniform vec3 u_light_color; // 灯光的颜色
uniform vec3 u_camera_pos_in_world; // 摄像机在世界空间中的位置坐标

// x分量为是否是否使用金属度贴图，y分量为粗糙度，大于0表示使用，小于0表示不适用
// z分量为如果不使用金属度贴图的话，金属度参数为多少
// w分量为如果不使用粗糙度贴图的话，粗糙度参数为多少
uniform vec4 u_metallic_roughness; 

const float PI = 3.14159265359;

// 从法线贴图中取出并且
vec3 GetNormalFromTexture()
{
    // 从法线贴图中，取得本片元对应的法线值，因为从图片中取得
    // 的法线值是RGB值，其取值范围是[0,1]。而转换到切线空间后
    // 取值范围是[-1,1]。所以要做一个映射关系的转换。
    vec3 tangentNormal = texture(u_normal_map, fragment_tex_coord_1).xyz * 2.0 - 1.0;
    
    /*
    求解TBN的方法有两种，一种是在片元着色器中使用叉乘计算TBN矩阵；另外一种是在根据
    顶点预计算TBN然后VAO中传入TBN矩阵，理论上来说后者会快一些。在本示例中，生成的
    球体自身是没有带切线信息的。所以还是需要在片元着色器中，利用片元在世界坐标系
    下的坐标，以及纹理映射坐标，出切线来。如何构建？就是要使用dFdx指令和dFdy指令
    
    
    偏导数函数（HLSL中的ddx和ddy，GLSL中的dFdx和dFdy）是只能用在片元着色器中的函数。
    在GL中，还有另外一个函数fwidth = dFdx + dFdy。
    在三角形栅格化期间，GPU会同时跑片元着色器的多个实例，但并不是一个pixel一个pixel
    去执行的，而是将其组织在2x2的一组pixels块中并行执行。偏导数就是通过像素块中的变
    量的差值（变化率）而计算出来的。dFdx表示的是像素块中右边像素的值减去素块中左边像
    素的值，而dFdy表示的是下面像素的值减去上面像素的值。如示意图所示：
    
    ---------------------------------------------
    |          |          |          |          |
    |          |          |          |          |
    |          |          |          |          |
    ---------------------------------------------
    |          |          |          |          |
    |          |  p(x,y)  | p(x+1,y) |          |
    |          |          |          |          |
    ---------------------------------------------
    |          |          |          |          |
    |          | p(x,y+1) |p(x+1,y+1)|          |
    |          |          |          |          |
    ---------------------------------------------
    |          |          |          |          |
    |          |          |          |          |
    |          |          |          |          |
    ---------------------------------------------

    dFdx(p(x,y)) = p(x+1,y)-p(x,y)
    dFdy(p(x,y)) = p(x,y+1)-p(x,y)

    */
    // dFdX和dFdX既然是近似于在该点的偏导数，根据导数的几何含义，
    // 一个点的导数表征的就是在该点上的斜率，也就是过该点的切线
    // 因为dFdx得到的是相对于X轴的切线，dFdy是相对于Y轴的切线
    // 所以在下面要用两个切线的差作为过本点的切线
    vec3 Q1  = dFdx(fragment_world_pos);    // 水平方向上的切线向量
    vec3 Q2  = dFdy(fragment_world_pos);    // 垂直方向上的切线向量

    // 构造切线空间，N是法线，T是切线，B是副法线
    vec3 N   = normalize(fragment_normal);
    vec3 T  = normalize(Q1-Q2); /* 因为过一点有很多切线，
                                    所以选在x轴和在y轴上的切线*/
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

/*在微观尺度下，没有任何平面是完全光滑的。然而由于这些微平面已经微小
到无法逐像素的继续对其进行区分，因此我们只有假设一个粗糙度(Roughness)
参数，然后用统计学的方法来概略的估算微平面的粗糙程度。我们可以基于一个
平面的粗糙度,来计算出某个向量的方向与微平面平均取向方向一致的概率。这个
向量便是位于光线向量l和视线向量v之间的中间向量(Halfway Vector)。它的计
算方法如下：

中间向量H = （l+v)/|l+v|。即是l向量和v向量之和，然后规格化之。

正态分布函数D，或者说镜面分布，从统计学上近似的表示了与某些（中间）
向量h取向一致的微平面的比率。举例来说，假设给定向中间量h，如果我们的
微平面中有35%与向量h取向一致，则正态分布函数或者说NDF将会返回0.35。
目前有很多种NDF都可以从统计学上来估算微平面的总体取向度，只要给定一些
粗糙度的参数以及一个我们马上将会要用到的参数。正态分布函数D的计算方式为：

D(h,n,roughess) = A /{PI [(n.h)(n.h)(A-1)+1][(n.h)(n.h)(A-1)+1]}
其中A = roughess * roughess

h就是中间向量，n就是宏观法线值，roughess就是表main粗糙度

当粗糙度很低（也就是说表面很光滑）的时候，与中间向量取向一致的微平面会
高度集中在一个很小的半径范围内。由于这种集中性，正态分布函数D最终会生成
一个非常明亮的斑点。但是当表面比较粗糙的时候，微平面的取向方向会更加的
随机。将会发现与h向量取向一致的微平面分布在一个大得多的半径范围内，
但是同时较低的集中性也会让最终效果显得更加灰暗。

使用GLSL代码编写的Trowbridge-Reitz GGX正态分布函数是下面这个样子的：
N 宏观法线
H 中间变量
roughness 表面粗糙度
*/
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float roughness_square = roughness*roughness;
    float normal_dot_halfway = max(dot(N, H), 0.0);
    float normal_dot_halfway_square = normal_dot_halfway * normal_dot_halfway;
    float denominator = (normal_dot_halfway_square * (roughness_square - 1.0) + 1.0);
    denominator = PI * denominator * denominator;
    return roughness_square / denominator;
}

/*
几何函数采用一个材料的粗糙度参数作为输入参数，粗糙度较高的表面其微平面间相互遮蔽的概率就
越高。这里的几何函数是GGX与Schlick-Beckmann近似的结合体，因此又称为Schlick-GGX：

GSchlickGGX(n,v,k) = (n.v)/[(n.v)(1-k)+k]

这里的系数k，依据使用直接光照计算，还是使用IBL去计算，其计算方法有所不同。
如果是直接光照计算，k = (roughness + 1.0)(roughness + 1.0) / 8
如果是基于IBL的话 k = roughness . roughness / 2

参数NdotV，如果传进来的，是表面宏观法线和摄像机到片元的连线向量的点积。这个就是
表征一部分微表面反射出来的光线被另一部分微表面挡住的，导致摄像机看不到的“蒙盖项”

参数NdotV，如果传进来的，是表面宏观法线和光源到片元的连线向量的点积。这个就是
表征一部分微表面遮挡住光线，使原本应该被光照的一部分微表面无法被照到的“遮蔽项”
*/
// NdotV 法线和视线的点积
// roughness 粗糙度
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    float denominator = NdotV * (1.0 - k) + k;
    return NdotV / denominator;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2; // 整体的遮挡-蒙盖函数由遮蔽项和蒙盖项相乘而来
}

/*
菲涅尔方程返回的是一个物体表面光线被反射的百分比，当光线照射到物体表面时（无论是金属，水）
。总有一部分是原样反射出来，有一部分是进入到物体内部去（也即是折射进去）。现在我们考虑两
种介质，一种是空气，一种是水。光线从空气射向水中。

菲涅尔效应就是，当一束能量为E的光，从光疏介质1射向光密介质2的时候，如果入射方向和法线的夹
角越大，则折射进介质2的光的能量Erefract越小，而在两介质相交面处反射出来的光的能量Ereflect
就越大。

举例说明：假设有一个手电筒，垂直于水面向水照射，而我们的视线也是垂直地观看水面这时候，我
们只会看到水底的石头，这是因为手电筒发射的光线，全部进入到水中，碰到石头再反射出来。也就
是说，这个时候，Ereflect基本上为0，Erefract基本上就等于E。而如果当光线近乎平行于水面射入
水面，且我们也是近乎平行水面地观察光线进入水面的情形。这时候我们会发现，基本上看不到水底
的石头了，而是看到手电筒发出的光斑。这时候，Erefract基本为0，Ereflect就基本等于E。

令折射进水里的能量与总能量的比值为：Krefract
令反射进水里的能量与总能量的比值为：Kreflect

则有Krefract + Kreflect = 1

Kreflect = (R1*R1+R2*R2)/2 其中:
R1 = [ηcos(θi)-cos(θt)]/[ηcos(θi)+cos(θt)]
R2 = [cos(θi)-ηcos(θt)]/[cos(θi)+ηcos(θt)]

η代表了该物体的相对折射率
θi和θt分别代表了入射角与折射角

基本上，当光线从光疏介质（空气）射入光密介质（水）的时候，当入射角越大时，其反射能量占比
就越大，折射进光密介质中的能量就越小。

而从光线从光密介质射入光疏介质的时候，因为在入射角达到一定程度的时候，会发生全反射现象。
当发生全反射现象时，此公式不适用。

公式中的kr与kt分别代表了最终求得的反射率与折射率，η代表了该物体的相对折射率，θi和θt分别代
表了入射角与折射角。可以看出对于透明物体而言，有多少光能被折射是跟物体的相对折射率以及入
射角度都是相关的，值得注意的一点是，当发生全反射的时候这个公式并不适用。

原始的菲涅尔方程比较复杂，所以可以采用一个近似的公式去代替它，即Fresnel-Schlick。
该公式为：

F(n,v,F0) = F0 + (1-F0) pow(1-dot(n,v),5)，其中
n是表面的宏观法线向量
v是表面到摄像机的连线向量

其中F0就是垂直观察平面时的基础反射率，因为对同一介质对不同波长的光的基本反射率是不同的，所
以会使用个vec3类型的量来表示

菲涅尔方程还存在一些细微的问题。其中一个问题是Fresnel-Schlick近似仅仅对电介质或者说非金属
表面有定义。对于导体表面（金属），使用它们的折射指数计算基础折射率并不能得出正确的结果，
这样我们就需要使用一种不同的菲涅尔方程来对导体表面进行计算。由于这样很不方便，所以预先计
算出平面对于法向入射（也就是入射方向和法线平行）的反应（处于0度角，好像直接看向表面一样，
这也就是上面的F0系数）然后基于相应观察角的Fresnel-Schlick近似对这个值进行插值，用这种方法
来进行进一步的估算。这样我们就能对金属和非金属材质使用同一个公式了——这也就是为什么要专门
引入一个金属度参数的缘故

这里可以观察到的一个有趣的现象，所有电介质材质表面的基础反射率都不会高于0.17，这其实是例
外而非普遍情况。导体材质表面的基础反射率起点更高一些并且（大多）在0.5和1.0之间变化。此外，
对于导体或者金属表面而言基础反射率一般是带有色彩的，这也是为什么要用RGB三原色来表示的原因
（法向入射的反射率可随波长不同而不同）。这种现象我们只能在金属表面观察的到。

金属表面这些和电介质表面相比所独有的特性引出了所谓的金属工作流的概念。也就是我们需要额外
使用一个被称为金属度(Metalness)的参数来参与编写表面材质的原因了。金属度用来描述一个材质
表面是金属还是非金属的。

理论上来说，一个表面的金属度应该是二元的：要么是金属要么不是金属，不能两者皆是。但是，大
多数的渲染管线都允许在0.0至1.0之间线性的调配金属度。这主要是由于材质纹理精度不足以描述一
个拥有诸如细沙/沙状粒子/刮痕的金属表面。通过对这些小的类非金属粒子/刮痕调整金属度值，我们
可以获得非常好看的视觉效果。

下面的函数中：
参数F0就是基础反射率
参数cos_theta就是表面宏观法向量和观察方向的点乘结果
*/
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
    float metallic  = 0.0f; 

    if (u_metallic_roughness.x > 0 ) // 使用金属度贴图
    {
        metallic = texture(u_metallic_map, fragment_tex_coord_1).r;
    }
    else
    {
        metallic = u_metallic_roughness.z;
    }

    float roughness  = 0.0f; 

    if (u_metallic_roughness.y > 0 ) // 使用金属度贴图
    {
        roughness = texture(u_roughness_map, fragment_tex_coord_1).r;
    }
    else
    {
        roughness = u_metallic_roughness.w;
    }

    // 拿到环境遮蔽系数
    float ao = texture(u_ambient_occlusion_map, fragment_tex_coord_1).r;
    
    vec3 albedo = pow(texture(u_albedo_map, fragment_tex_coord_1).rgb, vec3(2.2));

    vec3 N = GetNormalFromTexture(); // 拿到当前片元的法线
    vec3 V = normalize(u_camera_pos_in_world - fragment_world_pos); /* 拿到当前片元的在世
                                                        界空间中的位置到摄像机连线*/

    vec3 F0 = u_base_reflectivity;

    /*
    通过预先计算电介质与导体的值，我们可以对两种类型的表面使用相同的Fresnel-Schlick近似，
    但是如果是金属表面的话就需要对基础反射率添加色彩。我们一般是按下面这个样子来实现的：

    mix(F0, albedo, metallic)的结果是 F0 * (1-metallic) + metallic * albedo;

    我们为大多数电介质表面定义了一个近似的基础反射率。取最常见的电解质表面的平均值，这又
    是一个近似值。不过对于大多数电介质表面而言使用0.04作为基础反射率已经足够好了，而且可
    以在不需要输入额外表面参数的情况下得到物理可信的结果。然后，基于金属表面特性，我们要
    么使用电介质的基础反射率要么就使用来作为表面颜色。因为金属表面会吸收所有折射光线而没
    有漫反射，所以我们可以直接使用表面颜色纹理来作为它们的基础反射率。而如果是非金属的话，
    那么基础反射率就主要由F0提供

    下面的这句代码，也就是说，如果完全是金属的话，用于菲涅尔反射的基础反射率就使用反照率
    贴图提供的RGB值，而如果全不是金属的话，就使用一个提供的常量
    */
    F0 = mix(F0, albedo, metallic);

    vec3 frag_to_light = u_light_pos_in_world - fragment_world_pos;
    vec3 L = normalize(frag_to_light);// 计算从片元在世界空间的位置到光源的连线向量
    vec3 H = normalize(V + L); // 计算半角向量
    float distance = length(frag_to_light);
    float attenuation = 1.0 / (distance * distance); // 点光源，计算光的衰减度
    vec3 radiance = u_light_color * attenuation; // 入射辐射度就是用光的颜色诚意衰减度

    // Cook-Torrance BRDF 模型
    float D = DistributionGGX(N, H, roughness);       // 算出D项，法线朝向分布概率
    float G = GeometrySmith(N, V, L, roughness);        // 算出G项，遮挡蒙盖选项
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);   /* 算出F项，得到镜面反射出来的那
                                                        一部分能量占了总能量的百分比 */

    vec3 nominator    = D * G * F; 
    float denominator = 4 * max(dot(N, V), 0.0) * 
                            max(dot(N, L), 0.0) + 0.001; // 加一个0.001防止分母为0

    vec3 specular = nominator / denominator; // 得到镜面反射部分的BRDF计算子

    vec3 kS = F; // kS项就是Fresnel项。也是镜面反射的占比系数

    // 根据能量守恒，漫反射占比系数为1减去镜面反射占比系数。
    vec3 kD = vec3(1.0) - kS;

    /*因为金属不会折射光线，即光不会从空气或者别的介质中射入到金属，内部因此不会有漫反射。
    所以如果表面是金属的，利用金属度参数控制一下kD。从下面的式子可以看到，当金属度为1的时
    候KD就为1了，结合上面求反照率颜色的计算公式：F0 = mix(F0, albedo, metallic);可以看到
    全部反射出去了，没有漫反射项*/
    kD *= 1.0 - metallic;

    // 计算光线和法线之间的夹角
    float NdotL = max(dot(N, L), 0.0);

    // 正式计算出射度
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

    // 把环境遮蔽也一起做了
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    // 最终颜色为环境色 + 光线出射度
    vec3 color = ambient + Lo;

    /*
    假设所有计算都在线性的颜色空间中进行的，因此我们需要在着色器最后做伽马矫正。PBR对光照
    的计算要求在线性空间中完成。因为PBR要求所有输入都是线性的，就会得到不正常的光照。另外
    ，我们希望所有光照的输入都尽可能的接近他们在物理上的取值。反射率或者说颜色值就会在色
    谱上有比较大的变化空间。Lo作为结果可能会变大得很快(超过1)，但是因为默认的LDR输入而取
    值被截断。所以在伽马矫正之前我们采用色调映射使Lo从LDR的值映射为HDR的值。
    
    HDR的细节文档参阅：
    http://www.xionggf.com/post/cg/inside_hdr/
    */
    
    color = color / (color + vec3(1.0)); // 色调映射
    color = pow(color, vec3(1.0/2.2));  // 伽马校正
    fragment_color = vec4(color, 1.0);
}