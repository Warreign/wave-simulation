#include "mesh.h"

Mesh::Mesh(Shader* shader, uint8_t flags, int numPrimitives, int numVertices, long setSize)
	: shader(shader), flags(flags), numVertices(numVertices), numPrimitives(numPrimitives), vertexSetSize(setSize)
{
}

Mesh::Mesh(float* data, unsigned int* indices, unsigned int numPrimitives, unsigned int numVertices, Shader* shader, uint8_t flags)
	: shader(shader), numPrimitives(numPrimitives), numVertices(numVertices), flags(flags), vertexSetSize(numPrimitives * 3 * sizeof(float))
{
	initOffsets();
	initBuffers();

	setPositionData(data);
	if (flags & NORMAL_BIT)
		setNormalData((char*)data + normalOffset);
	if (flags & COLOR_BIT)
		setColorData((char*)data + colorOffset);
	if (flags & TEXTURE_BIT)
		setTexData((char*)data + texOffset);
}

void Mesh::initOffsets()
{
	bool normal = flags & NORMAL_BIT;
	bool color = flags & COLOR_BIT;
	bool tex = flags & TEXTURE_BIT;

	colorOffset = 3 * vertexSetSize;
	normalOffset = (normal + color) * 3 * vertexSetSize;
	texOffset = (normal + color + tex) * 3 * vertexSetSize;
}

void Mesh::initBuffers()
{
	bool normal = flags & NORMAL_BIT;
	bool color = flags & COLOR_BIT;
	bool tex = flags & TEXTURE_BIT;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(shader->attributes.position);
	glVertexAttribPointer(shader->attributes.position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (normal)
	{
		glEnableVertexAttribArray(shader->attributes.normal);
		glVertexAttribPointer(shader->attributes.normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)normalOffset);
	}

	if (color)
	{
		glEnableVertexAttribArray(shader->attributes.color);
		glVertexAttribPointer(shader->attributes.color, 3, GL_FLOAT, GL_FALSE, 0, (void*)colorOffset);
	}

	if (tex)
	{
		glEnableVertexAttribArray(shader->attributes.texCoord);
		glVertexAttribPointer(shader->attributes.texCoord, 2, GL_FLOAT, GL_FALSE, 0, (void*)texOffset);
	}

	glBufferData(GL_ARRAY_BUFFER, vertexSetSize * (3 + 3 * normal + 3 * color + 2 * tex), nullptr, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setData(void* data, size_t size, size_t offset, GLuint newVbo) const
{
	if (newVbo != 0)
		glBindBuffer(GL_ARRAY_BUFFER, newVbo);
	else
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::setPositionData(void* data) const
{
	setData(data, 3*vertexSetSize, 0);
}

void Mesh::setNormalData(void* data) const
{
	setData(data, 3*vertexSetSize, normalOffset);
}

void Mesh::setColorData(void* data) const
{
	setData(data, 3*vertexSetSize, colorOffset);
}

void Mesh::setTexData(void* data) const
{
	setData(data, 3*vertexSetSize, texOffset);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Mesh::draw(GLenum polygonMode) const
{
	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	glDrawArrays(GL_TRIANGLES, 0, 3 * numPrimitives);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

