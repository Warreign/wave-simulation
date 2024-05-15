#pragma once

#include "utils/shaderBase.h"

#include <iostream>

class ProfileCompute : public ShaderBase
{
public:
	ProfileCompute(const std::string& path);

	void loadUniforms();
	void dispatch();

private:
	std::string m_path;
};
