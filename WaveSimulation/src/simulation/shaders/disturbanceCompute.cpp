#include "disturbanceCompute.h"

#include <iostream>

DisturbanceCompute::DisturbanceCompute(const std::string& path)
	: m_path(path)
{
	std::cout << "\t Initializing " << m_path << " compute shader" << std::endl;

	GLuint computeShader = compileShader(GL_COMPUTE_SHADER, m_path);
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
		std::cerr << '\t' << message << std::endl;

		glDeleteProgram(program);
		delete[] message;
	}
}

void DisturbanceCompute::dispatch(GLuint inTexture, GLuint outTexture)
{
	bind();
	setInteger("in_Grid", 1);
	glBindImageTexture(1, inTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	setInteger("out_Grid", 2);
	glBindImageTexture(2, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
	glDispatchCompute(4, 1, 1);
	//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	unbind();
}

void DisturbanceCompute::loadUniforms(glm::ivec2 realPos, glm::ivec2 dim, float ampVal)
{
	setIVec2("u_position", realPos);
	setIVec2("u_dim", dim);
	setFloat("u_value", ampVal);
}
