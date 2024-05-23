#include "water.h"

// Water Object

Water::Water(WaterMesh* mesh)
	: ObjectInstance(mesh, glm::mat4(1.0f)),
	mesh(mesh)
{
	glm::vec3 ambient(0.25f, 0.2f, 0.23f);
	glm::vec3 diffuse(0.4f, 0.4f, 0.8f);
	glm::vec3 specular(1.0f);
	float shininess = 80.0f;
	mesh->shader->setColor(ambient, diffuse, specular, shininess);
}

void Water::draw(const Camera& camera, GLenum polygonMode) const
{
	mesh->shader->bind();
	mesh->shader->setTransformParameters(camera.projectMatrix(), camera.viewMatrix(), glm::mat4(1.0f), camera.position);

	mesh->shader->setColor(ambient, diffuse, specular, shininess);

	glm::vec3 lightPosition = glm::vec3(2, 2, 3);
	mesh->shader->setVec3("u_lightPosition", lightPosition);

	mesh->draw(polygonMode);
	mesh->shader->unbind();
}
