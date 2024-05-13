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
	~Shader();

	/// Load uniform and attribute location variables
	virtual void setLocations();
	/// Set transform matrices and camera position
	virtual void setTransformParameters(const glm::mat4& project, const glm::mat4& view, const glm::mat4& model, const glm::vec3& cameraPosition) const;
};