#pragma once

#include "visualization/objects/object.h"
#include "visualization/meshes/waterMesh.h"

class Water : ObjectInstance
{
protected:
	WaterMesh* mesh;

public:
	glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse = glm::vec3(0.1f, 0.4f, 0.8f);
	glm::vec3 specular = glm::vec3(1.0f);
	float shininess = 64.0f;

	Water(WaterMesh* mesh);

	void draw(const Camera& camera, GLenum polygonMode = GL_FILL) const override;
};