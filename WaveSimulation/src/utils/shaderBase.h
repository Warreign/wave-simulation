#pragma once

#include "utils/parameters.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string>
#include <format>
#include <iostream>
#include <initializer_list>
#include <regex>

class ShaderBase
{
public:
	virtual ~ShaderBase();

	static void addIncludeFile(const std::string& path);

	void bind() const;
	void unbind() const;

	/// Set uniforms by name

	void setIntegerv(const std::string& uniformName, int num, int* val);
	void setUInteger(const std::string uniformName, uint32_t value) const;
	void setInteger(const std::string uniformName, int value) const;
	void setIVec2(const std::string uniformName, glm::ivec2& value) const;
	void setIVec3(const std::string uniformName, glm::ivec3& value) const;
	void setIVec4(const std::string uniformName, glm::ivec4& value) const;
	void setFloat(const std::string uniformName, float value) const;
	void setVec2(const std::string uniformName, glm::vec2& value) const;
	void setVec3(const std::string uniformName, glm::vec3& value) const;
	void setVec4(const std::string uniformName, glm::vec4& value) const;
	void setMat4(const std::string uniformName, glm::mat4& value) const;


	/// Set uniforms by location
	void setInteger(GLint location, int value) const;
	void setIVec2(GLint location, glm::ivec2& value) const;
	void setIVec3(GLint location, glm::ivec3& value) const;
	void setIVec4(GLint location, glm::ivec4& value) const;
	void setFloat(GLint location, float value) const;
	void setVec2(GLint location, glm::vec2& value) const;
	void setVec3(GLint location, glm::vec3& value) const;
	void setVec4(GLint location, glm::vec4& value) const;
	void setMat4(GLint location, glm::mat4& value) const;

protected:
	static const std::string readShaderFromFile(const std::string& path);
	template<class... Files>
	static const GLuint compileShader(GLenum type, const std::string& path, const Files&... files);

	GLint uniformLocation(const std::string& name);
	GLint attribLocation(const std::string& name);

	GLuint program;


};

template<class ...Files>
inline const GLuint ShaderBase::compileShader(GLenum type, const std::string& path, const Files & ...files)
{
	GLint result;
	GLuint shader = glCreateShader(type);

	bool includes = false;
	([&] {
		includes = true;
		const std::string& fpath = files;
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
		} (), ...);

	std::string shaderSource = readShaderFromFile(path);
	const GLchar* source = shaderSource.c_str();
	glShaderSource(shader, 1, &source, nullptr);
	const char* search_directories[] = { "/", "/shaders" };
	glCompileShaderIncludeARB(shader, _countof(search_directories), search_directories, NULL);

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
