#pragma once
#include "cgp/cgp.hpp"

cgp::mesh procedural_tree(float length,float radius, int iteration, cgp::vec3 position1, cgp::vec3 position2 );
cgp::mesh create_procedural_tree_mesh(float length, float radius, int iteration, cgp::vec3 position);
