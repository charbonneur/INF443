#include "volcano.hpp"

using namespace cgp;
float evaluate_volcano_height(float x, float y, float volcano_length, float volcano_height, float radius)
{

    vec2 p = { 0.0f,0.0f };

    float h = -volcano_height / 10;
    float sigma = volcano_length;
    float h_0 = volcano_height;
    float sigma_0 = radius + (-radius + volcano_length) / 3.0f;
    float d = norm(vec2(x, y) - p) / sigma;
    float d_0 = norm(vec2(x, y) - p) / sigma_0;
    if (std::pow(x, 2.0f) + std::pow(y, 2.0f) > std::pow(radius, 2.0f)) {
        float z = h * std::exp(-d * d) + h_0 * std::exp(-d_0 * d_0);
        return z;
    }
    else {
        float z = h;
        return z;
    }


}

mesh create_volcano_mesh(int N, float volcano_length, float volcano_height, float volcano_radius) {
    mesh volcano; // temporary volcano storage (CPU only)
    volcano.position.resize(N * N);
    volcano.uv.resize(N * N);
    // Fill volcano geometry
    for (int ku = 0; ku < N; ++ku)
    {
        for (int kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku / (N - 1.0f);
            float v = kv / (N - 1.0f);

            // Compute the real coordinates (x,y) of the volcano 
            float x = (u - 0.5f) * volcano_length;
            float y = (v - 0.5f) * volcano_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_volcano_height(x, y, volcano_length, volcano_height, volcano_radius);

            // Store vertex coordinates
            volcano.position[kv + N * ku] = { x,y,z };
            volcano.uv[kv + N * ku] = { u,v };
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for (int ku = 0; ku < N - 1; ++ku)
    {
        for (int kv = 0; kv < N - 1; ++kv)
        {
            unsigned int idx = kv + N * ku; // current vertex offset

            uint3 triangle_1 = { idx, idx + 1 + N, idx + 1 };
            uint3 triangle_2 = { idx, idx + N, idx + 1 + N };

            volcano.connectivity.push_back(triangle_1);
            volcano.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
    volcano.fill_empty_field();

    return volcano;
}