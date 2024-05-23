#pragma once

#include "visualization/meshes/cubeMesh.h"
#include "visualization/objects/object.h"

class Cube : public ObjectInstance
{
public:
	Cube(Shader* shader, glm::mat4 model);
	~Cube();

	void draw(const Camera& camera, GLenum polygonMode = GL_FILL) const override;
};