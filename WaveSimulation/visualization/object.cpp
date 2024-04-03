#include "object.h"

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
		//if (!pgr::loadTexImage2D(path + "/" + faces[i] + "." + ext, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i)) {
		//	throw std::runtime_error("could not load skybox file: " + faces[i] + "." + ext);
		//}
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
