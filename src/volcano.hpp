#pragma once
#include "cgp/cgp.hpp"


cgp::mesh create_volcano_mesh(int N, float volcano_length, float volcano_height, float volcano_radius);
float evaluate_volcano_height(float x, float y, float volcano_length, float volcano_height, float radius);