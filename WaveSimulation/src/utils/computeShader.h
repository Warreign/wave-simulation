#pragma once

#include "utils/shaderBase.h"

class ComputeShader : public ShaderBase
{
public:
	template<class... Paths>
	ComputeShader(const std::string& path, const Paths&... includes);

private:
	std::string m_path;
};

template<class ...Paths>
inline ComputeShader::ComputeShader(const std::string& path, const Paths & ...includes)
	: m_path(path)
{
	std::cout << "\t Initializing " << m_path << " compute shader" << std::endl;

	GLuint computeShader = compileShader(GL_COMPUTE_SHADER, m_path, includes...);
	assert(computeShader != 0);

	program = glCreateProgram();

	glAttachShader(program, computeShader);
	glLinkProgram(program);

	glDeleteShader(computeShader);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

		GLchar* message = new GLchar[logSize];
		glGetProgramInfoLog(program, logSize, &logSize, message);

		std::cerr << "ERROR: Program " << m_path << " failed to link: " << std::endl;
		std::cerr << message << std::endl;

		glDeleteProgram(program);
		delete[] message;
	}
}
