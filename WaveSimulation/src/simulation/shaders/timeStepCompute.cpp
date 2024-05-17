#include "timeStepCompute.h"

#include <iostream>

TimeStepCompute::TimeStepCompute(const std::string path)
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
		std::cerr << message << std::endl;

		glDeleteProgram(program);
		delete[] message;
	}
}

void TimeStepCompute::dispatchAdvection(GLuint inTexture, GLuint outTexture, glm::ivec3 dim)
{
	bind();
	setInteger("in_Grid", 1);
	//glBindImageTexture(1, inTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	glBindTextureUnit(1, inTexture);
	setInteger("out_Grid", 2);
	glBindImageTexture(2, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

	glDispatchCompute(dim.x / 16, dim.y / 16, dim.z / 4);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	//glBindImageTexture(1, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	glBindTextureUnit(1, 0);
	glBindImageTexture(2, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);

	unbind();
}

void TimeStepCompute::dispatchDiffusion(GLuint inTexture, GLuint outTexture, glm::ivec3 dim)
{
	bind();
	setInteger("in_Grid", 1);
	glBindImageTexture(1, inTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	//glBindTextureUnit(1, inTexture);
	setInteger("out_Grid", 2);
	glBindImageTexture(2, outTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

	glDispatchCompute(dim.x/16, dim.y/16, dim.z/4);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindImageTexture(1, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	//glBindTextureUnit(1, 0);
	glBindImageTexture(2, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);

	unbind();
}

void TimeStepCompute::loadUniforms(glm::ivec3 dim, glm::vec3 min, glm::vec3 delta, float groupSpeed, float dt)
{
	bind();
	//glm::vec3 dim3(m_dim);
	setIVec3("u_dim", dim);
	setVec3("u_min", min);
	setVec3("u_delta", delta);
	setFloat("u_groupSpeed", groupSpeed);
	setFloat("u_dt", dt);
	unbind();
}
