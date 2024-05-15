#pragma once

#include "utils/shaderBase.h"

#include <iostream>

class ProfileCompute : public ShaderBase
{
public:
	ProfileCompute(const std::string& path);

	void loadUniforms(float kmin, float kmax, float time, float period, uint32_t resolution);
	void dispatch(GLuint profileTexture, uint32_t resolution);

private:
	std::string m_path;
};
