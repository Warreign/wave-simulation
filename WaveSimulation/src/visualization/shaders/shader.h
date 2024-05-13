#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

/// Generic shader object
class Shader
{
protected:
	GLuint program;
	std::string m_vertPath, m_fragPath;

	GLint uniformLocation(const std::string& name);
	GLint attribLocation(const std::string& name);

	static const std::string readShaderFromFile(const std::string& path);
	static const GLuint compileShader(GLenum type, const std::string& path);

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
	~Shader();

	void bind() const;
	void unbind() const;

	/// Load uniform and attribute location variables
	virtual void setLocations();
	/// Set transform matrices and camera position
	virtual void setTransformParameters(const glm::mat4& project, const glm::mat4& view, const glm::mat4& model, const glm::vec3& cameraPosition) const;

	/// Set uniforms by name
	void setInteger(const std::string uniformName, int value) const;
	void setFloat(const std::string uniformName, float value) const;
	void setVec2(const std::string uniformName, glm::vec2& value) const;
	void setVec3(const std::string uniformName, glm::vec3& value) const;
	void setMat4(const std::string uniformName, glm::mat4& value) const;

	/// Set uniforms by location
	void setInteger(GLint location, int value) const;
	void setFloat(GLint location, float value) const;
	void setVec2(GLint location, glm::vec2& value) const;
	void setVec3(GLint location, glm::vec3& value) const;
	void setMat4(GLint location, glm::mat4& value) const;
};