#include "shader.h"

#include <stdexcept>
#include <glm/ext.hpp>
#include <iostream>
#include <fstream>
#include <streambuf>

/*
*	Generic Shader
*/

const std::string Shader::readShaderFromFile(const std::string& path)
{
	std::ifstream fs(path);
	std::string shaderSource;

	fs.seekg(0, std::ios::end);
	shaderSource.reserve(fs.tellg());
	fs.seekg(0, std::ios::beg);

	shaderSource.assign(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
	fs.close();

	return shaderSource;
}

const GLuint Shader::compileShader(GLenum type, const std::string& path)
{
	GLint result;
	GLuint shader = glCreateShader(type);

	std::string shaderSource;
	shaderSource += readShaderFromFile(path);

	const GLchar* source = shaderSource.c_str();
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint logSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		GLchar* message = new GLchar[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, message);

		std::cerr << "ERROR: Shader " << path << " failed to compile: " << std::endl;
		std::cerr << message << std::endl;

		glDeleteShader(shader);
		delete[] message;
		return 0;
	}

	return shader;
}

GLint Shader::uniformLocation(const std::string& name)
{
	GLint loc = glGetUniformLocation(program, name.c_str());
	return loc;
}

GLint Shader::attribLocation(const std::string& name)
{
	GLint loc = glGetAttribLocation(program, name.c_str());
	return loc;
}

Shader::Shader()
	: program(0), attributes({ -1 }), uniforms({ -1 })
{
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	: m_vertPath(vertexShaderPath), m_fragPath(fragmentShaderPath), program(0)
{
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath);
	assert(vertexShader != 0);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);
	assert(fragmentShader != 0);

	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

		GLchar* message = new GLchar[logSize];
		glGetProgramInfoLog(program, logSize, &logSize, message);

		std::cerr << "ERROR: Program " << vertexShaderPath << "/" << fragmentShaderPath << " failed to link: " << std::endl;
		std::cerr << message << std::endl;

		glDeleteProgram(program);
		delete[] message;
	}
}

Shader::~Shader()
{
	//pgr::deleteProgramAndShaders(program);
	glDeleteProgram(program);
}

void Shader::bind() const
{
	glUseProgram(program);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setLocations()
{
	attributes.position = attribLocation("aPosition");
	attributes.color = attribLocation("aColor");
	attributes.normal = attribLocation("aNormal");
	attributes.texCoord = attribLocation("aTexCoord");

	uniforms.PVM = uniformLocation("PVM");
	uniforms.ViewM = uniformLocation("ViewM");
	uniforms.ModelM = uniformLocation("ModelM");
	uniforms.ProjectM = uniformLocation("ProjectM");
}

void Shader::setTransformParameters(const glm::mat4& project, const glm::mat4& view, const glm::mat4& model, const glm::vec3& cameraPosition) const
{
	glProgramUniformMatrix4fv(program, uniforms.PVM, 1, GL_FALSE, glm::value_ptr(project * view * model));
	glProgramUniformMatrix4fv(program, uniforms.ViewM, 1, GL_FALSE, glm::value_ptr(view));
	glProgramUniformMatrix4fv(program, uniforms.ModelM, 1, GL_FALSE, glm::value_ptr(model));
	glProgramUniformMatrix4fv(program, uniforms.ProjectM, 1, GL_FALSE, glm::value_ptr(project));
}

void Shader::setInteger(const std::string uniformName, int value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform1i(program, location, value);
}

void Shader::setFloat(const std::string uniformName, float value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform1f(program, location, value);
}

void Shader::setVec2(const std::string uniformName, glm::vec2& value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform2fv(program, location, 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string uniformName, glm::vec3& value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniform3fv(program, location, 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string uniformName, glm::mat4& value) const
{
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setInteger(GLint location, int value) const
{
	glProgramUniform1i(program, location, value);
}

void Shader::setFloat(GLint location, float value) const
{
	glProgramUniform1f(program, location, value);
}

void Shader::setVec2(GLint location, glm::vec2& value) const
{
	glProgramUniform2fv(program, location, 1, glm::value_ptr(value));
}

void Shader::setVec3(GLint location, glm::vec3& value) const
{
	glProgramUniform3fv(program, location, 1, glm::value_ptr(value));
}

void Shader::setMat4(GLint location, glm::mat4& value) const
{
	glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, glm::value_ptr(value));
}


/*
*	Water Shader
*/

WaterShader::WaterShader()
	: Shader("shaders/water.vert", "shaders/water.frag")
{
	setLocations();
}

void WaterShader::setLocations()
{
	attributes.position = attribLocation("aPosition");
	attributes.amplitudes = attribLocation("aAmplitudes");

	uniforms.PVM = uniformLocation("PVM");
	uniforms.ViewM = uniformLocation("ViewM");
	uniforms.ModelM = uniformLocation("ModelM");
	uniforms.NormalM = uniformLocation("NormalM");
	uniforms.ProjectM = uniformLocation("ProjectM");
	uniforms.lightPosition = uniformLocation("lightPosition");
	uniforms.cameraPosition = uniformLocation("cameraPosition");

	uniforms.ambient = uniformLocation("ambient");
	uniforms.diffuse = uniformLocation("diffuse");
	uniforms.specular = uniformLocation("specular");
	uniforms.shininess = uniformLocation("shininess");

	uniforms.profileBuffer = uniformLocation("profileBuffer");
	uniforms.profilePeriod = uniformLocation("profilePeriod");
}

void WaterShader::setTransformParameters(const glm::mat4& project, const glm::mat4& view, const glm::mat4& model, const glm::vec3& cameraPosition) const
{
	glProgramUniformMatrix4fv(program, uniforms.PVM, 1, GL_FALSE, glm::value_ptr(project * view * model));
	glProgramUniformMatrix4fv(program, uniforms.ViewM, 1, GL_FALSE, glm::value_ptr(view));
	glProgramUniformMatrix4fv(program, uniforms.ModelM, 1, GL_FALSE, glm::value_ptr(model));
	glProgramUniformMatrix4fv(program, uniforms.ProjectM, 1, GL_FALSE, glm::value_ptr(project));
	glProgramUniformMatrix4fv(program, uniforms.NormalM, 1, GL_FALSE, glm::value_ptr(glm::inverse(glm::transpose(view * model))));
	glProgramUniform3fv(program, uniforms.cameraPosition, 1, glm::value_ptr(cameraPosition));
}

void WaterShader::setColor(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float shininess)
{
	glProgramUniform3fv(program, uniforms.ambient, 1, glm::value_ptr(ambient));
	glProgramUniform3fv(program, uniforms.diffuse, 1, glm::value_ptr(diffuse));
	glProgramUniform3fv(program, uniforms.specular, 1, glm::value_ptr(specular));
	glProgramUniform1f(program, uniforms.shininess, shininess);
}
