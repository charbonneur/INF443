
#include "perlin_rock.hpp"
#include "cgp/shape/mesh/primitive/mesh_primitive.hpp"
#include "cgp/math/math.hpp"

//Pour faire bouger la mer
using namespace cgp;


mesh create_terrain_mesh(float length)
{
	int const terrain_sample = 180;
	mesh terrain = mesh_primitive_grid(length * vec3{ -1,-1,0 }, length * vec3{ 1,-1,0 }, length * vec3{ 1,1,0 }, length * vec3{ -1,1,0 }, terrain_sample, terrain_sample);
	return terrain;
}

float perlin_noise(float x, float y, perlin_noise_parameters const& parameters) {
	return noise_perlin({ x, y }, parameters.octave, parameters.persistency, parameters.frequency_gain);
}

void update_terrain(mesh& terrain, mesh_drawable& terrain_visual, perlin_noise_parameters const& parameters, float t)
{
	// Number of samples in each direction (assuming a square grid)
	int const N = std::sqrt(terrain.position.size());

	// Recompute the new vertices
	for (int ku = 0; ku < N; ++ku) {
		for (int kv = 0; kv < N; ++kv) {

			// Compute local parametric coordinates (u,v) \in [0,1]
			const float u = ku / (N - 1.0f);
			const float v = kv / (N - 1.0f);

			int const idx = ku * N + kv;

			// Compute the Perlin noise
			float const noise = perlin_noise(u - t, v, parameters);

			// use the noise as height value
			terrain.position[idx].z = noise;

			// use also the noise as color value
			terrain.color[idx] = 0.3f * parameters.color1 + 0.7f * noise * parameters.color2;
		}
	}

	

	// Update step
	terrain_visual.update_position(terrain.position);
	terrain_visual.update_color(terrain.color);

}

