#pragma once

#include "visualization/shaders/shader.h"
#include "visualization/camera.h"
#include "visualization/meshes/mesh.h"
#include "simulation/profile_buffer.h"
#include "simulation/amplitude_grid.h"

#include <iostream>
#include <filesystem>

bool loadTexImage2D(const std::string& path, GLenum target);

class ObjectInstance
{
protected:
	Mesh* geometry;

	glm::mat4 model;

	std::vector<ObjectInstance*> children;

public:
	ObjectInstance();
	ObjectInstance(Mesh* geometry, glm::mat4 model);

	glm::vec3 position;

	virtual void draw(const Camera& camera, GLenum polygonMode = GL_FILL) const;
};

