#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string>

class ShaderBase
{
public:
	void bind() const;
	void unbind() const;

	/// Set uniforms by name
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
	static const GLuint compileShader(GLenum type, const std::string& path);

	GLint uniformLocation(const std::string& name);
	GLint attribLocation(const std::string& name);

	GLuint program;


};