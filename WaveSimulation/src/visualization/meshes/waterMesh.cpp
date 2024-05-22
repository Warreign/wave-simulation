#include "waterMesh.h"

void WaterMesh::initOffsets()
{
}

void WaterMesh::initBuffers()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(shader->attributes.position);
	glVertexAttribPointer(shader->attributes.position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, vertexSetSize * 3, nullptr, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

WaterMesh::WaterMesh(WaterShader* shader, uint32_t size, float scale)
	: Mesh(shader, 0, size* size * 2, (size + 1)* (size + 1), (size + 1)* (size + 1) * sizeof(float)),
	shader(shader), size(size)
{

	initOffsets();
	initBuffers();

	float dx = scale / size;
	float dy = scale / size;

	positions.clear();
	positions.reserve(numVertices);
	for (int ix = 0; ix <= size; ++ix)
	{
		for (int iy = 0; iy <= size; ++iy)
		{
			positions.emplace_back(-scale / 2 + ix * dx, 0.0f, -scale / 2 + iy * dy);
		}
	}

	indices.clear();
	indices.reserve(numPrimitives * 3);
	for (int ix = 0; ix < size; ++ix)
	{
		for (int iy = 0; iy < size; ++iy)
		{
			int idx = iy + ix * (size + 1);
			int Ix = size + 1;
			int Iy = 1;
			indices.push_back(idx);
			indices.push_back(idx + Ix);
			indices.push_back(idx + Iy);

			indices.push_back(idx + Ix);
			indices.push_back(idx + Ix + Iy);
			indices.push_back(idx + Iy);
		}
	}

	setPositionData(positions.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

WaterMesh::~WaterMesh()
{
}

void WaterMesh::draw(GLenum polygonMode) const
{
	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}
