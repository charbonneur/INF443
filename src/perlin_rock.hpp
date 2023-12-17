#pragma once
#include "cgp/shape/mesh/primitive/mesh_primitive.hpp"
#include "cgp/math/math.hpp"


#include "cgp/cgp.hpp"

struct perlin_noise_parameters
{
	float persistency = 0.6f;
	float frequency_gain= 2.5f;
	int octave= 8;
	float terrain_height=30.0f;
	cgp::vec3 color1= { 0.96,0.89,0.75 };
	cgp::vec3 color2= { 0.96,0.89,0.75 };
};


// Initialize the mesh of the rock
cgp::mesh create_perlin_rock_mesh(float radius, cgp::vec3 const& center, int Nu, int Nv, perlin_noise_parameters const& parameters);

void update_rock(cgp::mesh& rock_mesh, cgp::mesh_drawable& rock, perlin_noise_parameters const& parameters, float radius, cgp::vec3 const& center, int Nu, int Nv);

float perlin_noise(float x, float y, perlin_noise_parameters const& parameters);
cgp::mesh create_terrain_mesh(float length);
void update_terrain(cgp::mesh& terrain, cgp::mesh_drawable& terrain_visual, perlin_noise_parameters const& parameters,float t);