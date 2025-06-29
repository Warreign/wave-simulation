#include "cubeMesh.h"

CubeMesh::CubeMesh(Shader* shader)
{
	this->shader = shader;
	numPrimitives = 12;
	numVertices = numPrimitives * 3;
	vertexSetSize = numVertices * sizeof(float);
	flags = NORMAL_BIT;
	float data[] = {
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
		 1.0f, -1.0f,  1.0f, 

		  0.0, 0.0, -1.0,
		  0.0, 0.0, -1.0,
		  0.0, 0.0, -1.0,
		  0.0, 0.0, -1.0,
		  0.0, 0.0, -1.0,
		  0.0, 0.0, -1.0,

		  -1.0, 0.0, 0.0,
		  -1.0, 0.0, 0.0,
		  -1.0, 0.0, 0.0,
		  -1.0, 0.0, 0.0,
		  -1.0, 0.0, 0.0,
		  -1.0, 0.0, 0.0,

		  1.0, 0.0, 0.0,
		  1.0, 0.0, 0.0,
		  1.0, 0.0, 0.0,
		  1.0, 0.0, 0.0,
		  1.0, 0.0, 0.0,
		  1.0, 0.0, 0.0,

		  0.0, 0.0, 1.0,
		  0.0, 0.0, 1.0,
		  0.0, 0.0, 1.0,
		  0.0, 0.0, 1.0,
		  0.0, 0.0, 1.0,
		  0.0, 0.0, 1.0,

		  0.0, 1.0, 0.0,
		  0.0, 1.0, 0.0,
		  0.0, 1.0, 0.0,
		  0.0, 1.0, 0.0,
		  0.0, 1.0, 0.0,
		  0.0, 1.0, 0.0,

		  0.0, -1.0, 0.0,
		  0.0, -1.0, 0.0,
		  0.0, -1.0, 0.0,
		  0.0, -1.0, 0.0,
		  0.0, -1.0, 0.0,
		  0.0, -1.0, 0.0
	};

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