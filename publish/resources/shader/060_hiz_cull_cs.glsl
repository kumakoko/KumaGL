/**************************************************************************************************************************
Copyright(C) 2014-2025 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#version 430 core

// 计算着色器

/*
本compute shader的工作流程是：
1 视锥剔除：首先对小球实例进行视锥剔除，如果小球实例不在视锥体内，则直接剔除。
2 近平面测试：如果小球实例与视截体近平面相交，则假设小球实例可见。
3 屏幕空间包围盒计算：计算小球实例在屏幕空间的包围盒。
4 LOD 计算：根据包围盒的大小计算合适的 LOD（Level of Detail）级别。
5 遮挡测试：在 Hi-Z 深度图中采样，判断小球实例是否被遮挡。
6 实例分类：根据深度值将小球实例分类到不同的 LOD 缓冲区中。
*/

precision highp float;
precision highp int;
precision highp sampler2DShadow;

layout(local_size_x = 64) in;

layout(binding = 0, std140) uniform UBO
{
    mat4 uVP;		// 观察-投影矩阵
    mat4 uView;		// 观察矩阵
    vec4 uProj[4];	// 投影矩阵
    vec4 uFrustum[6];	// 视截体的六个边的平面
    vec2 zNearFar;	// NearFar values for near plane intersection test.
};

layout(location = 0) uniform uint uNumBoundingBoxes;	// 显式地用location或者binding指定变量或资源的逻辑位置。
layout(binding = 0) uniform sampler2DShadow uDepth;	// 输入参数是纹理，uniform block，buffer等等，就要用binding关键字

// 每个LOD级别的原子计数器
layout(binding = 0, offset = 0) uniform atomic_uint instanceCountLOD0;
layout(binding = 1, offset = 0) uniform atomic_uint instanceCountLOD1;
layout(binding = 2, offset = 0) uniform atomic_uint instanceCountLOD2;
layout(binding = 3, offset = 0) uniform atomic_uint instanceCountLOD3;


struct SphereInstance
{
    vec4 position;
    vec4 velocity;
};

/*
//通常与 std140 或 std430 布局一起使用，以控制内存对齐。
// std140：OpenGL 标准的对齐规则。
// std430：更紧凑的对齐规则，适用于 SSBO（Shader Storage Buffer Object）。

binding = 0：指定缓冲区的绑定点为 0。
绑定点是 OpenGL 中用于将缓冲区对象（Buffer Object）与着色器中的 SSBO 关联的索引。
在应用程序中，可以通过 glBindBufferBase 或 glBindBufferRange 将缓冲区绑定到指定的绑定点。

buffer：声明一个 SSBO（Shader Storage Buffer Object）。SSBO 是一种可读写的缓冲区，允许着色器直接访问 GPU 上的大量数据。
与 Uniform Buffer Object (UBO) 不同，SSBO 的大小可以动态调整，并且支持读写操作。

PerInstanceInput：缓冲区的名称。这是一个自定义名称，用于在着色器中引用该缓冲区。类似于C语言里的一个struct的名字

readonly：指定缓冲区中的数据是只读的。这意味着着色器只能读取该缓冲区的数据，而不能修改它。如果需要写入数据，可以使用 writeonly 或省略修饰符（默认可读写）。

SphereInstance data[]：定义一个数组 data，其元素类型为 SphereInstance。SphereInstance 是一个结构体，表示每个实例的数据（如位置和速度）。data[] 是一个动态大小的数组，其大小由应用程序中绑定的缓冲区决定。

input_instance：缓冲区的实例名称。在着色器中，通过 input_instance.data[index] 访问缓冲区中的数据。例如，input_instance.data[0] 表示第一个实例的数据。
*/

/*
应用程序中的绑定
在 OpenGL 应用程序中，需要将缓冲区绑定到 SSBO 的绑定点。例如：

GLuint ssbo;
glGenBuffers(1, &ssbo);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SphereInstance) * numInstances, instanceData, GL_STATIC_DRAW);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo); // 绑定到绑定点 0

GL_SHADER_STORAGE_BUFFER：指定缓冲区类型为 SSBO。
glBindBufferBase：将缓冲区绑定到绑定点 0。
*/
layout(std430, binding = 0) buffer PerInstanceInput
{
    readonly SphereInstance data[];
} input_instance;

// We only need position as instance data.
layout(std430, binding = 1) buffer PerInstanceOutputLOD0
{
    writeonly vec4 data[];
} output_instance_lod0;

layout(std430, binding = 2) buffer PerInstanceOutputLOD1
{
    writeonly vec4 data[];
} output_instance_lod1;

layout(std430, binding = 3) buffer PerInstanceOutputLOD2
{
    writeonly vec4 data[];
} output_instance_lod2;

layout(std430, binding = 4) buffer PerInstanceOutputLOD3
{
    writeonly vec4 data[];
} output_instance_lod3;

// 根据深度值将实，将小球实例分类到不同的 LOD 缓冲区。
void append_instance(float minz)
{
    // 测试非线性深度值并将实例放置在适当的实例缓冲区中。
    if (minz < 0.8)
    {
        uint count = atomicCounterIncrement(instanceCountLOD0);
        output_instance_lod0.data[count] = input_instance.data[gl_GlobalInvocationID.x].position;
    }
    else if (minz < 0.9)
    {
        uint count = atomicCounterIncrement(instanceCountLOD1);
        output_instance_lod1.data[count] = input_instance.data[gl_GlobalInvocationID.x].position;
    }
    else if (minz < 0.95)
    {
        uint count = atomicCounterIncrement(instanceCountLOD2);
        output_instance_lod2.data[count] = input_instance.data[gl_GlobalInvocationID.x].position;
    }
    else
    {
        uint count = atomicCounterIncrement(instanceCountLOD3);
        output_instance_lod3.data[count] = input_instance.data[gl_GlobalInvocationID.x].position;
    }
}



/*
 判断实例是否在视锥体内。
 视锥体的每个平面都可以表示为一个方程：
 Ax+By+Cz+D=0
 其中 (A, B, C) 是平面的法向量，D 是平面到原点的距离。在 GLSL 中，uFrustum 存储了这些平面，并且每个平面用一个 vec4 表示：(A, B, C, D)。

算法逻辑：
1.循环遍历视锥体的六个平面。
2.对于每个平面，如果球心到平面的距离 plane_distance 小于 -radius，即球体的某
  部分已经被平面“裁掉”，则返回 false，表示球体不在视锥体内。
3.如果球体通过了所有六个平面的检测，则返回 true，表示球体完全或部分在视锥体内。
*/
bool frustum_test(vec3 center, float radius)
{
    for (int f = 0; f < 6; f++)
    {
    	/*
	dot(uFrustum[f], vec4(center, 1.0))：这行代码计算球心到平面的位置关系。通过将球心坐标 (center.x, center.y, center.z) 
	与平面法向量做点积运算，得到球心到平面的距离。uFrustum[f] 是平面的 vec4，将球心 (center, 1.0) 作为 vec4 输入，1.0 代
	表的是齐次坐标。

	如果 plane_distance 小于 -radius，意味着球心距离该平面太远，且球体的外部超出了视锥体的范围，因此该球体可以被裁剪掉（不在视锥体内）。
	*/
        float plane_distance = dot(uFrustum[f], vec4(center, 1.0));
        if (plane_distance < -radius)
            return false;
    }
    return true;
}

void main()
{
    /*
	gl_GlobalInvocationID 是一个 uvec3 类型的变量，表示当前调用在全局计算空间中的唯一坐标。它的值由以下因素决定：
	工作组大小：通过 layout(local_size_x = X, local_size_y = Y, local_size_z = Z) 定义。
	工作组数量：通过 glDispatchCompute 或 glDispatchComputeIndirect 指定。
	gl_GlobalInvocationID 的计算公式为：
	gl_GlobalInvocationID = gl_WorkGroupID × gl_WorkGroupSize + gl_LocalInvocationID
	gl_GlobalInvocationID=gl_WorkGroupID×gl_WorkGroupSize+gl_LocalInvocationID
    */
    // 获取当前线程的全局 ID gl_GlobalInvocationID.x。
    // 如果 ID 超出实例数量 uNumBoundingBoxes，则直接返回。
    uint ident = gl_GlobalInvocationID.x;

    if (ident >= uNumBoundingBoxes)
        return;

    vec4 instance_data = input_instance.data[ident].position;
    vec3 center = instance_data.xyz;
    float radius = instance_data.w;

    // 检查小球实例是否在视截体内，不是的话就跳出，不检查了
    if (!frustum_test(center, radius))
        return;

    // 应用视图变换。相机指向 -Z 轴向下。
    // 小球将实例的球心转换到视图空间。
    // 计算球体到近平面的最近距离 nearest_z。
    // 如果球体与近平面相交，则假设实例可见，调用 append_instance 并返回。
    vec3 view_center = (uView * vec4(center, 1.0)).xyz;
    float nearest_z = view_center.z + radius;

    // 当小球夹在近平面上时，假设它是可见的

    if (nearest_z >= -zNearFar.x)
    {
        append_instance(0.0);
        return;
    }

    /*
	查找屏幕空间边界框。有关算法的更多详细信息，请参阅文档。
	该算法的思想是将球体投影到水平和垂直平面:
	然后我们得到一个带有圆的 2D 平面。在投影的 2D 空间中找到从相机到圆的切线，并使用它来计算与近平面相交的点。
	为了找到切点，应用勾股定理 length_tangent = sqrt(length_center^2 - radius^2)，使用它来创建 2D 旋转矩阵，并
	在两个方向上旋转以直接找到两个切点。从那里，可以进行透视除法来独立找到水平和垂直平面的最小/最大值。 min_xy 和 max_xy。
    */
    float az_plane_horiz_length = length(view_center.xz);
    float az_plane_vert_length = length(view_center.yz);
    vec2 az_plane_horiz_norm = view_center.xz / az_plane_horiz_length;
    vec2 az_plane_vert_norm = view_center.yz / az_plane_vert_length;

    vec2 t = sqrt(vec2(az_plane_horiz_length, az_plane_vert_length) * vec2(az_plane_horiz_length, az_plane_vert_length) - radius * radius);
    vec4 w = vec4(t, radius, radius) / vec2(az_plane_horiz_length, az_plane_vert_length).xyxy;

    // 一种优化方法，用于应用两个旋转矩阵。由于这两个旋转矩阵几乎相同（只是正弦函数的符号相反），因此可以重用部分计算。
    vec4 horiz_cos_sin = az_plane_horiz_norm.xyyx * t.x * vec4(w.xx, -w.z, w.z);
    vec4 vert_cos_sin  = az_plane_vert_norm.xyyx * t.y * vec4(w.yy, -w.w, w.w);

    vec2 horiz0 = horiz_cos_sin.xy + horiz_cos_sin.zw;
    vec2 horiz1 = horiz_cos_sin.xy - horiz_cos_sin.zw;
    vec2 vert0  = vert_cos_sin.xy + vert_cos_sin.zw;
    vec2 vert1  = vert_cos_sin.xy - vert_cos_sin.zw;

    //这假设投影矩阵首先不进行平移或任何其他变换。
    vec4 projected = -0.5 * vec4(uProj[0][0], uProj[0][0], uProj[1][1], uProj[1][1]) *
        vec4(horiz0.x, horiz1.x, vert0.x, vert1.x) /
        vec4(horiz0.y, horiz1.y, vert0.y, vert1.y) + 0.5;

    // 因为我们知道要以哪个方向旋转来寻找切点，所以我们已经知道哪个是最小值，哪个是最大值。.
    vec2 min_xy = projected.yw;
    vec2 max_xy = projected.xz;

    // 在视图空间中投射我们最近的 Z 值。
    vec2 zw = mat2(uProj[2].zw, uProj[3].zw) * vec2(nearest_z, 1.0);
    nearest_z = 0.5 * zw.x / zw.y + 0.5;

    // 计算阴影查找所需的 LOD 因子。
    // 根据包围盒的大小计算 LOD 级别：
    // 计算包围盒在屏幕空间的像素大小 diff_pix。
    // 使用 内置函数log2 和 ceil 计算合适的 LOD 级别。

    // max_diff：表示实例在屏幕空间包围盒的最大边长（以像素为单位）。
    // log2(max_diff)：计算 max_diff 的以 2 为底的对数，表示所需的 LOD 级别。
    // ceil(log2(max_diff))：将 LOD 级别向上取整，确保它是一个整数。
    vec2 diff_pix = (max_xy - min_xy) * vec2(textureSize(uDepth, 0));
    float max_diff = max(max(diff_pix.x, diff_pix.y), 1.0);
    float lod = ceil(log2(max_diff)); // ceil函数返回大于或等于输入值的最小整数。
                                      // log2函数返回输入值的以 2 为底的对数。

    vec2 mid_pix = 0.5 * (max_xy + min_xy);

    /*
    // textureLod函数从纹理中采样，并指定显式的 LOD（Level of Detail）级别。
    // uDepth：Hi-Z 深度图，用于遮挡测试。
    // mid_pix：实例在屏幕空间包围盒的中心坐标。
    // nearest_z：实例在视图空间中的最近深度值。
    // lod：计算得到的 LOD 级别。
    // 作用：在 Hi-Z 深度图中采样，判断实例是否被遮挡。如果采样值大于 0.0，则实例可见。
    */
    if (textureLod(uDepth, vec3(mid_pix, nearest_z), lod) > 0.0) 
        append_instance(nearest_z);
}

