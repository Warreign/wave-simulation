#pragma once

#include "visualization/meshes/mesh.h"
#include "visualization/shaders/waterShader.h"
#include "simulation/amplitudeGrid.h"
#include "utils/parameters.h"

class WaterMesh : public Mesh
{
public:

	WaterMesh(WaterShader* shader, uint32_t size, float scale);
	~WaterMesh();

	WaterShader* shader;
protected:

	uint32_t size;

	std::vector<glm::vec3> positions;
	std::vector<unsigned int> indices;

	void initOffsets() override;
	void initBuffers() override;
public:

	void draw(GLenum polygonMode = GL_TRIANGLES) const override;
};