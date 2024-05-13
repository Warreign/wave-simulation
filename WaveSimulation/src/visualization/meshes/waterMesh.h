#pragma once

#include "visualization/meshes/mesh.h"
#include "visualization/shaders/waterShader.h"

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