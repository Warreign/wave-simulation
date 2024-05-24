/**
 * @file object.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "visualization/shaders/shader.h"
#include "visualization/camera.h"
#include "visualization/meshes/mesh.h"
#include "simulation/amplitudeGrid.h"

#include <iostream>
#include <filesystem>

/*
* load an image to currently bound texture target
* 
* @param path file where image is stored
* @param target target to load the image into
*/
bool loadTexImage2D(const std::string& path, GLenum target);


/*
* Generic object
*/
class ObjectInstance
{
protected:
	// Mesh associated wiht the object
	Mesh* geometry;

	// Model matrix of the object
	glm::mat4 model;

	// Children of the object
	std::vector<ObjectInstance*> children;

public:

	ObjectInstance();
	/*
	* Initialize the object from mesh
	* 
	* @param mesh obejct mesh
	* @param model model matrix for the object
	*/
	ObjectInstance(Mesh* geometry, glm::mat4 model);

	glm::vec3 position;

	/*
	* draw the object using its mesh and model matrix
	* 
	* @param camera active camera obejcts to get view and projection matrices
	* @param polygonMode mode to draw
	*/
	virtual void draw(const Camera& camera, GLenum polygonMode = GL_FILL) const;
};

