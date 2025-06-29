/**
 * @file object.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "visualization/objects/object.h"


/*
* Object representing skybox
*/
class Skybox : ObjectInstance
{
protected:
	// skybox texture object
	GLuint m_texture;
public:

	/*
	* load all 6 images for each side of cube sampler
	* 
	* @param path path to folder containing all the images
	*/
	static GLuint loadTexture(const std::string& path);

	/*
	* Construct a cube mesh and load the texture
	*/
	explicit Skybox();
	Skybox(const std::string& folderPath);

	~Skybox();

	GLuint getTexture() { return m_texture; }

	/*
	* draw skybox 
	*/
	void draw(const Camera& camera, GLenum polygonMode = GL_FILL) const override;
};