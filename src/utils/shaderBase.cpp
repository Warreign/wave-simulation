#include "shaderBase.h"

#include <glm/ext.hpp>

#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>

ShaderBase::~ShaderBase()
{
	glDeleteProgram(program);
}

void ShaderBase::addIncludeFile(const std::string& fpath)
{
	std::cout << "\t Adding " << fpath << " as shader include file" << std::endl;

	std::string content = readShaderFromFile(fpath);

	std::regex macro_regex(".*macros.*", std::regex::extended);
	if (std::regex_search(fpath, macro_regex))
	{
		std::stringstream strstrm(content);
		std::string line;
		std::vector<std::string> lines;
		while (std::getline(strstrm, line, '\n'))
			lines.push_back(line);
		lines.pop_back();
		lines.pop_back();
		std::string nodef = "";
		for (auto l : lines)
			nodef += l + '\n';

		content = nodef;
		content += std::format("#define N_THETA {}\n", N_THETA);
		content += std::format("#define N_K {}\n", N_K);
	}

	std::string glpath = "/" + fpath;
	glNamedStringARB(GL_SHADER_INCLUDE_ARB, -1, glpath.c_str(), -1, content.c_str());
}

void ShaderBase::bind() const
{
	glUseProgram(program);
}

void ShaderBase::unbind() const
{
	glUseProgram(0);
}

void ShaderBase::setIntegerv(const std::string& uniformName, int num, int* val)
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform1iv(program, location, num, val);
}

void ShaderBase::setUInteger(const std::string uniformName, uint32_t value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform1ui(program, location, value);
}

void ShaderBase::setInteger(const std::string uniformName, int value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform1i(program, location, value);
}

void ShaderBase::setIVec2(const std::string uniformName, glm::ivec2& value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform2iv(program, location, 1, glm::value_ptr(value));
}

void ShaderBase::setIVec3(const std::string uniformName, glm::ivec3& value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform3iv(program, location, 1, glm::value_ptr(value));
}

void ShaderBase::setIVec4(const std::string uniformName, glm::ivec4& value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform4iv(program, location, 1, glm::value_ptr(value));
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

void ShaderBase::setVec4(const std::string uniformName, glm::vec4& value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform4fv(program, location, 1, glm::value_ptr(value));
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

void ShaderBase::setIVec2(GLint location, glm::ivec2& value) const
{
	glProgramUniform2iv(program, location, 1, glm::value_ptr(value));
}

void ShaderBase::setIVec3(GLint location, glm::ivec3& value) const
{
	glProgramUniform3iv(program, location, 1, glm::value_ptr(value));
}

void ShaderBase::setIVec4(GLint location, glm::ivec4& value) const
{
	glProgramUniform4iv(program, location, 1, glm::value_ptr(value));
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

void ShaderBase::setVec4(GLint location, glm::vec4& value) const
{
	glProgramUniform4fv(program, location, 1, glm::value_ptr(value));
}

void ShaderBase::setMat4(GLint location, glm::mat4& value) const
{
	glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
}


const std::string ShaderBase::readShaderFromFile(const std::string& path)
{
	std::ifstream fs(path);
	std::string shaderSource;

	if (!fs)
	{
		std::cerr << "\t File " << path << " does not exist!" << std::endl;
		return "";
	}

	fs.seekg(0, std::ios::end);
	shaderSource.reserve(fs.tellg());
	fs.seekg(0, std::ios::beg);

	shaderSource.assign(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
	fs.close();

	return shaderSource;
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


