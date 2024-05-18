#pragma once

#include <utils/shaderBase.h>

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

/// Generic shader object
class Shader : public ShaderBase
{
protected:
	std::string m_vertPath, m_fragPath;

	static inline const std::string GLSL_VERSION = "#version 460 core\n";

public:
	struct Attributes {
		GLint position;
		GLint color;
		GLint normal;
		GLint texCoord;
	} attributes;

	struct Uniforms {
		GLint PVM;
		GLint ViewM;
		GLint ModelM;
		GLint ProjectM;
	} uniforms;

	Shader();
	Shader(const std::string& vertexFile, const std::string& fragmentFile);

	template<class... Files>
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const Files&... files);

	/// Load uniform and attribute location variables
	virtual void setLocations();
	/// Set transform matrices and camera position
	virtual void setTransformParameters(const glm::mat4& project, const glm::mat4& view, const glm::mat4& model, const glm::vec3& cameraPosition) const;
};

template<class... Files>
Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const Files&... files)
	: m_vertPath(vertexShaderPath), m_fragPath(fragmentShaderPath)
{
	std::cout << "\t Initializing " << vertexShaderPath << "|" << fragmentShaderPath << std::endl;

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath, files...);
	assert(vertexShader != 0);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath, files...);
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