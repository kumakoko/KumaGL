/*!
 * \file light_render_pass.cc
 * \date 2017/12/22 14:53
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

#include "light_render_pass.h"

namespace kgl
{
	DSLightPassTech::DSLightPassTech()
	{
	}

	bool DSLightPassTech::Init()
	{
		m_WVPLocation = GetUniformLocation("gWVP");
		m_posTextureUnitLocation = GetUniformLocation("gPositionMap");
		m_colorTextureUnitLocation = GetUniformLocation("gColorMap");
		m_normalTextureUnitLocation = GetUniformLocation("gNormalMap");
		m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
		m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
		m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
		m_screenSizeLocation = GetUniformLocation("gScreenSize");

		if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
			m_posTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
			m_colorTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
			m_normalTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
			m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
			m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
			m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
			m_screenSizeLocation == INVALID_UNIFORM_LOCATION) {
			return false;
		}

		return true;
	}


	void DSLightPassTech::SetWVP(const Matrix4f& WVP)
	{
		glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
	}


	void DSLightPassTech::SetPositionTextureUnit(unsigned int TextureUnit)
	{
		glUniform1i(m_posTextureUnitLocation, TextureUnit);
	}


	void DSLightPassTech::SetColorTextureUnit(unsigned int TextureUnit)
	{
		glUniform1i(m_colorTextureUnitLocation, TextureUnit);
	}


	void DSLightPassTech::SetNormalTextureUnit(unsigned int TextureUnit)
	{
		glUniform1i(m_normalTextureUnitLocation, TextureUnit);
	}


	void DSLightPassTech::SetEyeWorldPos(const Vector3f& EyePos)
	{
		glUniform3f(m_eyeWorldPosLocation, EyePos.x, EyePos.y, EyePos.z);
	}


	void DSLightPassTech::SetScreenSize(unsigned int Width, unsigned int Height)
	{
		glUniform2f(m_screenSizeLocation, (float)Width, (float)Height);
	}


	
}

#endif // light_render_pass_h__
