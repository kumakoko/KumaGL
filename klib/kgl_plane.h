/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

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
/*!
 * \file kgl_plane.h
 * \date 2018/02/12 11:42
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: ƽ����
 *
 * \note
*/
#ifndef kgl_plane_h__
#define kgl_plane_h__

namespace kgl
{
	class Plane
	{
	private:
		/// <summary>
		/// ƽ����淨��
		/// </summary>
		glm::vec3 normal_;

		/// <summary>
		/// ƽ���ϵ�һ����
		/// </summary>
		glm::vec3 point_;

		/// <summary>
		/// �ؾ�
		/// </summary>
		float d_;
	public:
		inline const glm::vec3& Normal() const
		{
			return normal_;
		}

		/// <summary>
		/// ��<see cref="Plane"/>�Ĺ��캯��.��������ȷ��һ��ƽ��
		/// </summary>
		/// <param name="v1">The v1.</param>
		/// <param name="v2">The v2.</param>
		/// <param name="v3">The v3.</param>
		Plane(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);
		
		/// <summary>
		/// ��<see cref="Plane"/>��ȱʡ���캯��
		/// </summary>
		Plane();
		
		/// <summary>
		/// ��<see cref="Plane"/>��ȱʡ���캯��
		/// </summary>
		~Plane();

		/// <summary>
		/// ��������ȷ��һ��ƽ��
		/// </summary>
		/// <param name="v1">The v1.</param>
		/// <param name="v2">The v2.</param>
		/// <param name="v3">The v3.</param>
		void SetThreePoints(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);

		/// <summary>
		/// ��ƽ���ϵ�һ����ͷ���ȷ��һ��ƽ��
		/// </summary>
		/// <param name="normal">The normal.</param>
		/// <param name="point">The point.</param>
		void SetNormalAndPoint(const glm::vec3 &normal, const glm::vec3 &point);

		/// <summary>
		/// ����ƽ�淽��ax+by+cz+d=0���ĸ�ϵ��a��b��c��d
		/// ����a,b,c��ƽ����淨��
		/// </summary>
		/// <param name="a">a.</param>
		/// <param name="b">The b.</param>
		/// <param name="c">The c.</param>
		/// <param name="d">The d.</param>
		void SetCoefficients(float a, float b, float c, float d);

		/// <summary>
		/// ��P����ƽ��ľ���
		/// </summary>
		/// <param name="p">The p.</param>
		/// <returns>float.</returns>
		float Distance(const glm::vec3 &p);

		void InfoPrint();
	};
}
#endif // kgl_plane_h__