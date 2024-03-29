#pragma once

#include "pgr.h"
#include <vector>

class Grid {
protected:
	std::vector<float>  data;

public:
	glm::ivec4 dimensions;

	Grid();
	Grid(int n0, int n1, int n2, int n3);

	int dimension(int dim) const;
	void resize(int n0, int n1, int n2, int n3);
	float& operator()(int i0, int i1, int i2, int i3);
	float const& operator()(int i0, int i1, int i2, int i3) const;

};
