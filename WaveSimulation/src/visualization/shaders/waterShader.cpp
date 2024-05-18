#include "waterShader.h"

#include <glm/ext.hpp>

/*
*	Water Shader
*/

WaterShader::WaterShader()
	: Shader("shaders/water.vert", "shaders/water.frag", "shaders/water_macros.glsl", "shaders/water_common.glsl")
{
	setLocations();
}

void WaterShader::setLocations()
{
	attributes.position = attribLocation("aPosition");
	attributes.amplitudes = attribLocation("aAmplitudes");

	uniforms.PVM = uniformLocation("PVM");
	uniforms.ViewM = uniformLocation("ViewM");
	uniforms.ModelM = uniformLocation("ModelM");
	uniforms.NormalM = uniformLocation("NormalM");
	uniforms.ProjectM = uniformLocation("ProjectM");
	uniforms.lightPosition = uniformLocation("lightPosition");
	uniforms.cameraPosition = uniformLocation("cameraPosition");

	uniforms.ambient = uniformLocation("ambient");
	uniforms.diffuse = uniformLocation("diffuse");
	uniforms.specular = uniformLocation("specular");
	uniforms.shininess = uniformLocation("shininess");

	uniforms.profileBuffer = uniformLocation("profileBuffer");
	uniforms.profilePeriod = uniformLocation("profilePeriod");
}

void WaterShader::setTransformParameters(const glm::mat4& project, const glm::mat4& view, const glm::mat4& model, const glm::vec3& cameraPosition) const
{
	glProgramUniformMatrix4fv(program, uniforms.PVM, 1, GL_FALSE, glm::value_ptr(project * view * model));
	glProgramUniformMatrix4fv(program, uniforms.ViewM, 1, GL_FALSE, glm::value_ptr(view));
	glProgramUniformMatrix4fv(program, uniforms.ModelM, 1, GL_FALSE, glm::value_ptr(model));
	glProgramUniformMatrix4fv(program, uniforms.ProjectM, 1, GL_FALSE, glm::value_ptr(project));
	glProgramUniformMatrix4fv(program, uniforms.NormalM, 1, GL_FALSE, glm::value_ptr(glm::inverse(glm::transpose(view * model))));
	glProgramUniform3fv(program, uniforms.cameraPosition, 1, glm::value_ptr(cameraPosition));
}

void WaterShader::setColor(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float shininess)
{
	glProgramUniform3fv(program, uniforms.ambient, 1, glm::value_ptr(ambient));
	glProgramUniform3fv(program, uniforms.diffuse, 1, glm::value_ptr(diffuse));
	glProgramUniform3fv(program, uniforms.specular, 1, glm::value_ptr(specular));
	glProgramUniform1f(program, uniforms.shininess, shininess);
}
