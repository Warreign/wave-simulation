#include "profileCompute.h"

ProfileCompute::ProfileCompute(const std::string& path)
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

		std::cerr << "\t ERROR: Program " << m_path << " failed to link: " << std::endl;
		std::cerr << '\t' << message << std::endl;

		glDeleteProgram(program);
		delete[] message;
	}
}

void ProfileCompute::loadUniforms(float kmin, float kmax, float time, float period, uint32_t resolution)
{
	setFloat("u_kmin", kmin);
	setFloat("u_kmax", kmax);
	setFloat("u_time", time);
	setFloat("u_period", period);
	setUInteger("u_resolution", resolution);
}

void ProfileCompute::dispatch(GLuint profileTexture, uint32_t resolution)
{
	bind();
	setInteger("out_ProfileBuffer", 3);
	glBindImageTexture(3, profileTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glDispatchCompute(resolution/64, 1, 1);
	//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	unbind();
}
