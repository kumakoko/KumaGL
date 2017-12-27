/*!
 * \file light_render_pass.h
 * \date 2017/12/22 14:56
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef light_render_pass_h__
#define light_render_pass_h__

namespace kgl
{
	class DSLightPassTech
	{
	public:

		DSLightPassTech();

		virtual bool Init();

		void SetWVP(const Matrix4f& WVP);
		void SetPositionTextureUnit(unsigned int TextureUnit);
		void SetColorTextureUnit(unsigned int TextureUnit);
		void SetNormalTextureUnit(unsigned int TextureUnit);
		void SetEyeWorldPos(const Vector3f& EyeWorldPos);
		void SetMatSpecularIntensity(float Intensity);
		void SetMatSpecularPower(float Power);
		void SetScreenSize(unsigned int Width, unsigned int Height);

	protected:
		GPUProgramSPtr	lighting_shader_;
		GLuint			m_WVPLocation;
		GLuint			m_posTextureUnitLocation;
		GLuint			m_normalTextureUnitLocation;
		GLuint			m_colorTextureUnitLocation;
		GLuint			m_eyeWorldPosLocation;
		GLuint			m_matSpecularIntensityLocation;
		GLuint			m_matSpecularPowerLocation;
		GLuint			m_screenSizeLocation;
	};
}

#endif // light_render_pass_h__