#include "cube.h"

Cube::Cube(Shader* shader, glm::mat4 model)
	: ObjectInstance(new CubeMesh(shader), model)
{
	
}

Cube::~Cube()
{
	delete geometry;
}

void Cube::draw(const Camera& camera, GLenum polygonMode) const
{
	geometry->shader->bind();
	geometry->shader->setTransformParameters(camera.projectMatrix(), camera.viewMatrix(), model, camera.position);

	glm::vec3 lightPosition = glm::vec3(2, 2, 3);
	geometry->shader->setVec3("u_lightPosition", lightPosition);

	geometry->draw(polygonMode);
	geometry->shader->unbind();
}
