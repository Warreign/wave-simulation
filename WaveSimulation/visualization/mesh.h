#pragma once

//#include "pgr.h"
#include "shader.h"
#include "../simulation/amplitude_grid.h"
#include "../simulation/profile_buffer.h"

#include <algorithm>
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <array>

#define COLOR_BIT			0b0001
#define NORMAL_BIT			0b0010
#define TEXTURE_BIT			0b0100

class Mesh
{
protected:
	GLuint vbo;
	GLuint ebo;
	GLuint vao;

	uint8_t flags;

	long normalOffset;
	long colorOffset;
	long texOffset;
	long vertexSetSize;

	virtual void initOffsets();
	virtual void initBuffers();
	void setData(void* data, size_t size, size_t offset, GLuint vbo = 0) const;
	void setPositionData(void* data) const;
	void setNormalData(void* data) const;
	void setColorData(void* data) const;
	void setTexData(void* data) const;

	Mesh(Shader* shader, uint8_t flags, int numVertices, int numPrimitives, long setSize);

public:
	unsigned int numPrimitives;
	unsigned int numVertices;
	Shader* shader;

	Mesh(float* data, unsigned int* indices, unsigned int numPrimitives, unsigned int numVertices, Shader* shader, uint8_t flags);
	~Mesh();

	virtual void draw(GLenum polygonMode = GL_TRIANGLES) const;
};

class WaterMesh : public Mesh
{
public:
	using amplitude = std::array<float, NTHETA>;

	WaterMesh(WaterShader* shader, AmplitudeGrid* amplitudeGrid, uint32_t size, float scale);
	~WaterMesh();

	WaterShader* shader;
protected:

	GLuint amplitudeVbo;
	long amplitudeOffset;

	uint32_t size;
	GLuint profileTexture;
	AmplitudeGrid* amplitudeGrid;

	std::vector<glm::vec3> positions;
	std::vector<amplitude> amplitudes;
	std::vector<unsigned int> indices;

	void initOffsets() override;
	void initBuffers() override;

	void setAmplitudeData(void* data) const;

public:

	void updateData(float multiplier);
	void setProfileBuffer(const ProfileBuffer& profileBuffer) const;
	void draw(GLenum polygonMode = GL_TRIANGLES) const override;
};