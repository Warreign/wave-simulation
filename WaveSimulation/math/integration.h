#pragma once

//#include "pgr.h"

template <typename Fun>
auto integrate(int integration_nodes, double x_min, double x_max, Fun const& fun) {

    float dx = (x_max - x_min) / integration_nodes;
    float x = x_min + 0.5 * dx;

    auto result = dx * fun(x);
    for (int i = 1; i < integration_nodes; i++) {
        x += dx;
        result += dx * fun(x);
    }

    return result;
}