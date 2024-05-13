#pragma once

#include "visualization/objects/object.h"

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