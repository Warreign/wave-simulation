#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

/// Generic shader object
class Shader
{
protected:
	GLuint program;

	GLint uniformLocation(const std::string& name);
	GLint attribLocation(const std::string& name);

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
	Shader(std::string vertexFile, std::string fragmentFile);
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

class WaterShader : public Shader
{
public:

	struct Attributes {
		GLint position;
		GLint amplitudes;
	} attributes;

	struct Uniforms {
		GLint PVM;
		GLint ViewM;
		GLint ModelM;
		GLint NormalM;
		GLint ProjectM;

		GLint ambient;
		GLint diffuse;
		GLint specular;
		GLint shininess;

		GLint lightPosition;
		GLint cameraPosition;

		GLint profileBuffer;
		GLint profilePeriod;
		GLint gerstnerParameter;
	} uniforms;

	WaterShader();

	/// Load uniform and attribute location variables
	void setLocations() override;
	/// Set transform matrices and camera position
	void setTransformParameters(const glm::mat4& project, const glm::mat4& view, const glm::mat4& model, const glm::vec3& cameraPosition) const override;
	/// Set water color material parameters
	void setColor(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float shininess);
};
