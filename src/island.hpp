#pragma once
#include "cgp/cgp.hpp"


float evaluate_island_height(float x, float y, float radius);
cgp::mesh create_island_mesh(float length,bool curve);
cgp::mesh create_mountain_mesh(int N, float radius);

struct perlin_noise_island
{
	float persistency = 0.6f;
	float frequency_gain = 2.5f;
	int octave = 8;
	float terrain_height = 1.5f;

};