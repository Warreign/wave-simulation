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

Skybox::Skybox()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	auto shader = new Shader("shaders/skybox.vert", "shaders/skybox.frag");
	geometry = new Mesh(skyboxVertices, nullptr, 12, 8, shader, TEXTURE_BIT);

	this->geometry = geometry;
	this->texture = loadTexture("textures/skybox");
}

Skybox::Skybox(const std::string& folderPath)
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	auto shader = new Shader("shaders/skybox.vert", "shaders/skybox.frag");
	geometry = new Mesh(skyboxVertices, nullptr, 12, 8, shader, TEXTURE_BIT);

	this->geometry = geometry;
	this->texture = loadTexture(folderPath);
}

Skybox::~Skybox()
{
	delete(geometry->shader);
	delete(geometry);
	glDeleteTextures(1, &texture);
}

GLuint Skybox::loadTexture(const std::string& path) {
	GLuint skyboxTexture;
	glGenTextures(1, &skyboxTexture);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	std::filesystem::directory_entry file = *std::filesystem::directory_iterator(path);
	std::string filename = file.path().string();
	std::string ext = filename.substr(filename.find_last_of(".") + 1);
	std::vector<std::string> faces = {
		"px",
		"nx",
		"py",
		"ny",
		"pz",
		"nz"
	};

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < faces.size(); ++i) {
		if (!loadTexImage2D(path + "/" + faces[i] + "." + ext, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i)) {
			throw std::runtime_error("could not load skybox file: " + faces[i] + "." + ext);
		}
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(GL_TEXTURE0);

	return skyboxTexture;
}

void Skybox::draw(const Camera& camera, GLenum polygonMode) const
{
	glDepthFunc(GL_LEQUAL);

	geometry->shader->bind();
	glm::mat4 noTranslateView = glm::mat4(glm::mat3(camera.viewMatrix()));
	geometry->shader->setTransformParameters(camera.projectMatrix(), noTranslateView, model, camera.position);

	geometry->shader->setInteger("skybox", 10);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	geometry->draw(polygonMode);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(GL_TEXTURE0);

	geometry->shader->unbind();
	glDepthFunc(GL_LESS);
}

// Water Object

Water::Water(WaterMesh* mesh)
	: ObjectInstance(mesh, glm::mat4(1.0f)),
	mesh(mesh)
{
	glm::vec3 ambient(0.25f, 0.2f, 0.23f);
	glm::vec3 diffuse(0.4f, 0.4f, 0.8f);
	glm::vec3 specular(1.0f);
	float shininess = 80.0f;
	mesh->shader->setColor(ambient, diffuse, specular, shininess);
}

void Water::draw(const Camera& camera, GLenum polygonMode) const
{
	mesh->shader->bind();
	mesh->shader->setTransformParameters(camera.projectMatrix(), camera.viewMatrix(), glm::mat4(1.0f), camera.position);

	mesh->shader->setColor(ambient, diffuse, specular, shininess);

	glm::vec3 lightPosition = glm::vec3(2, 2, 3);
	mesh->shader->setVec3("lightPosition", lightPosition);

	mesh->draw(polygonMode);
	mesh->shader->unbind();
}
