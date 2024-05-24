/**
 * @file cubeMesh.h
 *
 * @author Viktor Shubert
 * Contact: shubevik@fel.cvut.cz
 *
 */

#pragma once

#include "visualization/meshes/mesh.h"
#include "visualization/shaders/shader.h"

class CubeMesh : public Mesh
{
public:
	CubeMesh(Shader* shader);


};