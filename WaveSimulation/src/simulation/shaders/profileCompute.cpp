#include "profileCompute.h"

ProfileCompute::ProfileCompute(const std::string& path)
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
