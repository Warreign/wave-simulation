/**
 * @file cube.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "visualization/meshes/cubeMesh.h"
#include "visualization/objects/object.h"


/*
* Cube object
*/
class Cube : public ObjectInstance
{
public: 
	/*
	* Construct a cube mesh and save it 
	* 
	* @param shader shader used to render
	* @param model model matrix used to position the cube
	*/
	Cube(Shader* shader, glm::mat4 model);
	~Cube();

	void draw(const Camera& camera, GLenum polygonMode = GL_FILL) const override;
};