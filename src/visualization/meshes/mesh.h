/**
 * @file mesh.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "visualization/shaders/shader.h"

#include <algorithm>
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <array>

// flags what the buffers for this mesh will store
#define COLOR_BIT			0b0001
#define NORMAL_BIT			0b0010
#define TEXTURE_BIT			0b0100

/*
* Generic mesh
*/
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

	// initialize offsets inside the buffer for all the attributes
	virtual void initOffsets();

	// initialize VBO and VAO
	virtual void initBuffers();
	// Set data inside the buffer
	void setData(void* data, size_t size, size_t offset, GLuint vbo = 0) const;
	// Set data on position offset
	void setPositionData(void* data) const;
	// Set data on normal offeset
	void setNormalData(void* data) const;
	// Set data on collor offset
	void setColorData(void* data) const;
	// Set data on tex coord offset
	void setTexData(void* data) const;

	Mesh() {}
	
	/*
	* Mesh constructor
	* 
	* @param shader shader associated with the mesh
	* @param flags data flags
	* @param numVertices number of vertices in the mesh
	* @param numPrimitives number of primitives in the mesh
	* @param setSize byte size of a vertex set
	*/
	Mesh(Shader* shader, uint8_t flags, int numVertices, int numPrimitives, long setSize);

public:
	unsigned int numPrimitives;
	unsigned int numVertices;
	Shader* shader;

	/*
	* Mesh constructor
	*
	* @param shader shader associated with the mesh
	* @param flags data flags
	* @param numVertices number of vertices in the mesh
	* @param numPrimitives number of primitives in the mesh
	* @param data pointer do data about the vertices
	* @param indices pointer to data about indices
	*/
	Mesh(float* data, unsigned int* indices, unsigned int numPrimitives, unsigned int numVertices, Shader* shader, uint8_t flags);
	~Mesh();

	/*
	* draw the mesh with its shader
	* 
	* @param polygonMode polygon mode to set during render
	*/
	virtual void draw(GLenum polygonMode = GL_TRIANGLES) const;
};
