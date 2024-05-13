#include "shaderBase.h"

#include <glm/ext.hpp>

#include <iostream>
#include <fstream>
#include <streambuf>

void ShaderBase::bind() const
{
	glUseProgram(program);
}

void ShaderBase::unbind() const
{
	glUseProgram(0);
}

void ShaderBase::setInteger(const std::string uniformName, int value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform1i(program, location, value);
}

void ShaderBase::setFloat(const std::string uniformName, float value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform1f(program, location, value);
}

void ShaderBase::setVec2(const std::string uniformName, glm::vec2& value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform2fv(program, location, 1, glm::value_ptr(value));
}

void ShaderBase::setVec3(const std::string uniformName, glm::vec3& value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform3fv(program, location, 1, glm::value_ptr(value));
}

void ShaderBase::setMat4(const std::string uniformName, glm::mat4& value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderBase::setInteger(GLint location, int value) const
{
	glProgramUniform1i(program, location, value);
}

void ShaderBase::setFloat(GLint location, float value) const
{
	glProgramUniform1f(program, location, value);
}

void ShaderBase::setVec2(GLint location, glm::vec2& value) const
{
	glProgramUniform2fv(program, location, 1, glm::value_ptr(value));
}

void ShaderBase::setVec3(GLint location, glm::vec3& value) const
{
	glProgramUniform3fv(program, location, 1, glm::value_ptr(value));
}

void ShaderBase::setMat4(GLint location, glm::mat4& value) const
{
	glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
}


const std::string ShaderBase::readShaderFromFile(const std::string& path)
{
	std::ifstream fs(path);
	std::string shaderSource;


	fs.seekg(0, std::ios::end);
	shaderSource.reserve(fs.tellg());
	fs.seekg(0, std::ios::beg);

	shaderSource.assign(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
	fs.close();

	return shaderSource;
}

const GLuint ShaderBase::compileShader(GLenum type, const std::string& path)
{
	GLint result;
	GLuint shader = glCreateShader(type);

	std::string shaderSource = readShaderFromFile(path);
	const GLchar* source = shaderSource.c_str();
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint logSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		GLchar* message = new GLchar[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, message);

		std::cerr << "ERROR: Shader " << path << " failed to compile: " << std::endl;
		std::cerr << message << std::endl;

		glDeleteShader(shader);
		delete[] message;
		return 0;
	}

	return shader;
}


GLint ShaderBase::uniformLocation(const std::string& name)
{
	GLint loc = glGetUniformLocation(program, name.c_str());
	return loc;
}

GLint ShaderBase::attribLocation(const std::string& name)
{
	GLint loc = glGetAttribLocation(program, name.c_str());
	return loc;
}

