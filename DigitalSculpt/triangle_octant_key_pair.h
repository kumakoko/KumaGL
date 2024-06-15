/*!
 * \file triangle_octant_key_pair.h
 * \date 2024/06/15 17:36
 *
 * \author  kumakoko
 * Contact: user@company.com
 *
 * \brief
 *
 * TODO: 定义了三角形索引值-八叉树元（octant）映射关系的key pair
 *
 * \note
*/
#ifndef triangle_octant_key_pair_h__
#define triangle_octant_key_pair_h__

namespace DigitalSculpt
{
    struct TriangleOctantKeyPair
    {
    public:
        TriangleOctantKeyPair() :triangleIndex(0), octantIndex(0)
        {
        }

        TriangleOctantKeyPair(int triPosition, int octIndex) :triangleIndex(triPosition), octantIndex(octIndex)
        {
        }

        ~TriangleOctantKeyPair()
        {
        }

        int triangleIndex;
        int octantIndex;
    };
}

#endif // triangle_octant_key_pair_h__