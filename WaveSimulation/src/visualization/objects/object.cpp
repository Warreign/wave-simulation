#include "object.h"

#include <cstdint>
#include <IL/il.h>

bool loadTexImage2D(const std::string& path, GLenum target)
{
	ILuint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);

	std::wstring wpath(path.begin(), path.end());
	const wchar_t* path_ptr = wpath.c_str();
	if (ilLoadImage(path_ptr) == IL_FALSE)
	{
		ilDeleteImages(1, &imgID);
		std::cout << ("Texture " + path + " failed to load," + std::to_string(ilGetError()) + "!\n") << std::endl;
		return false;
	}

	int m_width, m_height;
	uint8_t* m_data;

	m_width = ilGetInteger(IL_IMAGE_WIDTH);
	m_height = ilGetInteger(IL_IMAGE_HEIGHT);
	m_data = new uint8_t[m_width * m_height * 4];
	ilCopyPixels(0, 0, 0, m_width, m_height, 1, IL_RGBA, IL_UNSIGNED_BYTE, m_data);

	ilBindImage(0);
	ilDeleteImages(1, &imgID);

	//glTextureStorage2D(m_textureID, 1, GL_RGBA8, m_width, m_height);
	//glTextureSubImage2D(m_textureID, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_data);

	//glTexStorage2D(target, 1, GL_RGBA8, m_width, m_height);
	//glTexSubImage2D(target, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
	glTexImage2D(target, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);

	delete[] m_data;
	return true;
}

ObjectInstance::ObjectInstance()
	: geometry(nullptr), model(glm::mat4(1.0f)), position(glm::vec3(0.0f))
{
}

ObjectInstance::ObjectInstance(Mesh* geometry, glm::mat4 model) :
	geometry(geometry), model(model), position(model[3])
{
}

void ObjectInstance::draw(const Camera& camera, GLenum polygonMode) const
{
	geometry->shader->bind();
	geometry->shader->setTransformParameters(camera.projectMatrix(), camera.viewMatrix(), model, camera.position);
	geometry->draw(polygonMode);
	geometry->shader->unbind();
}