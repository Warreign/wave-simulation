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


void WaterMesh::initOffsets()
{
	amplitudeOffset = 3 * vertexSetSize;
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

	glGenBuffers(1, &amplitudeVbo);
	glBindBuffer(GL_ARRAY_BUFFER, amplitudeVbo);

	for (int i = 0; i < NTHETA/4; ++i)
	{
		glEnableVertexAttribArray(shader->attributes.amplitudes + i);
		glVertexAttribPointer(shader->attributes.amplitudes + i, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 4 * i));
	}

	glBufferData(GL_ARRAY_BUFFER, vertexSetSize * 16, nullptr, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WaterMesh::setAmplitudeData(void* data) const
{
	setData(data, vertexSetSize * 16, 0, amplitudeVbo);
}

WaterMesh::WaterMesh(WaterShader* shader, AmplitudeGrid* amplitudeGrid, uint32_t size, float scale)
	: Mesh(shader, 0, size * size * 2, (size+1) * (size+1), (size + 1)* (size + 1) * sizeof(float)),
	shader(shader), amplitudeGrid(amplitudeGrid), size(size)
{
	glGenTextures(1, &profileTexture);
	glBindTexture(GL_TEXTURE_1D, profileTexture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_1D, 0);

	initOffsets();
	initBuffers();

	float dx = scale / size;
	float dy = scale / size;

	positions.clear();
	positions.reserve(numVertices);
	amplitudes.reserve(numVertices);
	for (int ix = 0; ix <= size; ++ix)
	{
		for (int iy = 0; iy <= size; ++iy)
		{
			positions.emplace_back(-scale/2 + ix * dx, 0.0f, -scale/2 + iy * dy);
			amplitude a = { 0 };
			amplitudes.push_back(a);
		}
	}

	indices.clear();
	indices.reserve(numPrimitives*3);
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
	setAmplitudeData(amplitudes.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

WaterMesh::~WaterMesh()
{
	glDeleteTextures(1, &profileTexture);
	glDeleteBuffers(1, &amplitudeVbo);
}

void WaterMesh::updateData(float mult)
{
#ifdef NDEBUG
#pragma omp parallel for collapse(2)
#endif
	for (int ix = 0; ix <= size; ++ix)
	{
		for (int iz = 0; iz <= size; ++iz)
		{
			for (int itheta = 0; itheta < NTHETA; ++itheta)
			{
				//for (int ik = 0; ik <= amplitudeData->dim[3]; ++ik)
				//{

				//}
				int idx = ix * (size + 1) + iz;
				float theta = amplitudeGrid->realPos(itheta, 2);
				glm::vec3& pos = positions[idx];

				amplitudes[idx][itheta] = mult * amplitudeGrid->interpolatedValue(pos.x, pos.z, theta, amplitudeGrid->realPos(0, 3));
			}
		}
	}
	setAmplitudeData(amplitudes.data());
}

void WaterMesh::setProfileBuffer(const ProfileBuffer& profileBuffer) const 
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, profileTexture);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, profileBuffer.resolution(), 0, GL_RGBA, GL_FLOAT, profileBuffer.data());
	shader->setInteger("profileBuffer", 0);
	shader->setFloat("profilePeriod", profileBuffer.period);
}

void WaterMesh::draw(GLenum polygonMode) const
{
	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

