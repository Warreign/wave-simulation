#include "timeStepCompute.h"

#include <iostream>

TimeStepCompute::TimeStepCompute(const std::string path, int dimX, int dimZ, int dimTheta, int dimK)
	: m_path(path), m_dim(dimX, dimZ, dimTheta, dimK)
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

	glCreateTextures(GL_TEXTURE_3D, 1, &m_inTexture);
	glTextureParameteri(m_inTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_inTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_inTexture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_inTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_inTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureStorage3D(m_inTexture, 1, GL_R32F, dimX, dimZ, dimTheta);

	glCreateTextures(GL_TEXTURE_3D, 1, &m_outTexture);
	glTextureParameteri(m_outTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_outTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_outTexture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_outTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_outTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureStorage3D(m_outTexture, 1, GL_R32F, dimX, dimZ, dimTheta);
}

TimeStepCompute::~TimeStepCompute()
{
}

void TimeStepCompute::dispatch() const
{
	bind();
	setInteger("in_Grid", 1);
	glBindTextureUnit(1, m_inTexture);
	setInteger("out_Grid", 2);
	glBindTextureUnit(2, m_outTexture);

	glDispatchCompute(m_dim.x, m_dim.y, m_dim.z);
	unbind();
}

void TimeStepCompute::loadUniforms(glm::vec3 min, glm::vec3 delta, float groupSpeed, float dt)
{
	bind();
	setVec3("u_min", min);
	setVec3("u_delta", min);
	setFloat("u_groupSpeed", groupSpeed);
	setFloat("u_dt", dt);
	unbind();
}
