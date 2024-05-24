/**
 * @file waterMesh.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "visualization/meshes/mesh.h"
#include "visualization/shaders/lightingShader.h"
#include "simulation/amplitudeGrid.h"
#include "utils/parameters.h"


/*
* Mesh representing water surface
*/
class WaterMesh : public Mesh
{
public:


	/*
	* Same as normal mesh, but generates itself
	* 
	* @param shader lighting shader that implements phong model
	* @param size size in vertices of the water mesh
	* @param scale physical size of the mesh
	*/
	WaterMesh(LIghtingShader* shader, uint32_t size, float scale);
	~WaterMesh();

	LIghtingShader* shader;
protected:

	uint32_t size;

	std::vector<glm::vec3> positions;
	std::vector<unsigned int> indices;

	void initOffsets() override;
	void initBuffers() override;
public:

	void draw(GLenum polygonMode = GL_TRIANGLES) const override;
};