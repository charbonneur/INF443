#include "island.hpp"


using namespace cgp;

mesh create_island_mesh(float length,bool curve)
{
	int const terrain_sample = 180;
	mesh island = mesh_primitive_grid(vec3{ length,length ,0 }, vec3{ length,-length,0 }, vec3{ -length,-length ,0 }, vec3{ -length,length ,0 }, terrain_sample, terrain_sample);
	int const N = std::sqrt(island.position.size());
	for (int ku = 0; ku < N; ++ku) {
		for (int kv = 0; kv < N; ++kv) {

			// Compute local parametric coordinates (u,v) \in [0,1]
			const float u = ku / (N - 1.0f);
			const float v = kv / (N - 1.0f);

			int const idx = ku * N + kv;
			if (curve){
			island.position[idx].z = 8 * std::cos(v * 3.14 / 2) - 4.7; }
			else {
				island.position[idx].z = 3;
			}
		}
	}


	return island;
}

float evaluate_island_height(float x, float y, float radius)
{
    vec2 p_0 = { radius / 3,radius / 3 };
    vec2 p = { 0.0f,0.0f };
    float h_0 = 0.25f;
    float h = 10.0f;
    float sigma_0 = radius / 3.0f;
    float sigma = radius;

    float d_0 = norm(vec2(x, y) - p_0) / sigma_0;
    float d = norm(vec2(x, y) - p) / sigma;

    float z = h_0 * std::exp(-d_0 * d_0) + h * std::exp(-d * d);

    return z;
}
//
//
//
mesh create_mountain_mesh(int N, float length)
{
	int const terrain_sample = 180;
	mesh island = mesh_primitive_grid(vec3{ length,length ,0 }, vec3{ length,-length,0 }, vec3{ -length,-length ,0 }, vec3{ -length,length ,0 }, terrain_sample, terrain_sample);
	int const M = std::sqrt(island.position.size());
	for (int ku = 0; ku < N; ++ku) {
		for (int kv = 0; kv < N; ++kv) {

			// Compute local parametric coordinates (u,v) \in [0,1]
			const float u = ku / (N - 1.0f);
			const float v = kv / (N - 1.0f);

			int const idx = ku * N + kv;
			
			island.position[idx].z = evaluate_island_height(u, v, length);
			
		}
	}






    // need to call this function to fill the other buffer with default values (normal, color, etc)
    island.fill_empty_field();

   

    return island;
}