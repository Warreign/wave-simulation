#include "shader.h"

#include <stdexcept>
#include <glm/ext.hpp>
#include <iostream>

/*
*	Generic Shader
*/

Shader::Shader()
	: attributes({ -1 }), uniforms({ -1 })
{
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	: m_vertPath(vertexShaderPath), m_fragPath(fragmentShaderPath)
{
	std::cout << "\t Initializing " << vertexShaderPath << "|" << fragmentShaderPath << std::endl;

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath);
	assert(vertexShader != 0);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);
	assert(fragmentShader != 0);


	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

		GLchar* message = new GLchar[logSize];
		glGetProgramInfoLog(program, logSize, &logSize, message);

		std::cerr << "ERROR: Program " << vertexShaderPath << "/" << fragmentShaderPath << " failed to link: " << std::endl;
		std::cerr << message << std::endl;

		glDeleteProgram(program);
		delete[] message;
	}

	setLocations();
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& commonPath)
	: m_vertPath(vertexShaderPath), m_fragPath(fragmentShaderPath)
{
	std::cout << "\t Initializing " << vertexShaderPath << "|" << fragmentShaderPath << std::endl;

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath);
	assert(vertexShader != 0);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);
	assert(fragmentShader != 0);


	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

		GLchar* message = new GLchar[logSize];
		glGetProgramInfoLog(program, logSize, &logSize, message);

		std::cerr << "ERROR: Program " << vertexShaderPath << "/" << fragmentShaderPath << " failed to link: " << std::endl;
		std::cerr << message << std::endl;

		glDeleteProgram(program);
		delete[] message;
	}

	setLocations();
}

void Shader::setLocations()
{
	attributes.position = attribLocation("aPosition");
	attributes.color = attribLocation("aColor");
	attributes.normal = attribLocation("aNormal");
	attributes.texCoord = attribLocation("aTexCoord");

	uniforms.PVM = uniformLocation("PVM");
	uniforms.ViewM = uniformLocation("ViewM");
	uniforms.ModelM = uniformLocation("ModelM");
	uniforms.ProjectM = uniformLocation("ProjectM");
}

void Shader::setTransformParameters(const glm::mat4& project, const glm::mat4& view, const glm::mat4& model, const glm::vec3& cameraPosition) const
{
	glProgramUniformMatrix4fv(program, uniforms.PVM, 1, GL_FALSE, glm::value_ptr(project * view * model));
	glProgramUniformMatrix4fv(program, uniforms.ViewM, 1, GL_FALSE, glm::value_ptr(view));
	glProgramUniformMatrix4fv(program, uniforms.ModelM, 1, GL_FALSE, glm::value_ptr(model));
	glProgramUniformMatrix4fv(program, uniforms.ProjectM, 1, GL_FALSE, glm::value_ptr(project));
}
