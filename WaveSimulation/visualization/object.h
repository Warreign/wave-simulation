#pragma once

#ifndef _OBJECT_H
#define _OBJECT_H

#include "pgr.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "../simulation/profile_buffer.h"
#include "../simulation/amplitude_grid.h"
#include <iostream>
#include <filesystem>

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


class Skybox : ObjectInstance
{
protected:
	GLuint texture;
public:
	static GLuint loadTexture(const std::string& path);


	explicit Skybox();
	Skybox(const std::string& folderPath);

	~Skybox();

	void draw(const Camera& camera, GLenum polygonMode = GL_FILL) const override;
};

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

#endif