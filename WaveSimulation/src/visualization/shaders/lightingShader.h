/**
 * @file lightingShader.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "visualization/shaders/shader.h"

#include <glad/glad.h>



/*
* Generic shader that implements phong illumination model
*/
class LIghtingShader : public Shader
{
public:

	template<class... Paths>
	LIghtingShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const Paths&... includes);
	/// Set transform matrices and camera position
	void setTransformParameters(const glm::mat4& project, const glm::mat4& view, const glm::mat4& model, const glm::vec3& cameraPosition) const override;
	/// Set water color material parameters
	void setColor(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float shininess);

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

private:
	/// Load uniform and attribute location variables
	void setLocations() override;
};

template<class ...Paths>
inline LIghtingShader::LIghtingShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const Paths & ...includes)
	: Shader(vertexShaderPath, fragmentShaderPath, includes...)
{
	setLocations();
}
