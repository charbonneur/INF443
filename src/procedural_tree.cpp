#include "procedural_tree.hpp"


using namespace cgp;

mesh procedural_tree(float length, float radius, int iteration, vec3 position1, vec3 position2 ) {
	if (iteration == 0) {
		mesh branch;
		branch = mesh_primitive_cylinder(radius, position1, position2, 10, 20);
		branch.color.fill({ 0.45, 0.36, 0.26 });
		mesh leaf= mesh_primitive_cone(radius*2.5, 3.5*radius, position2,(position2-position1)/norm(position2-position1));
		leaf.color.fill({ rand_interval(0.30,0.6), rand_interval(0.40,0.60), 0});
		branch.push_back(leaf);
		return branch;
	}
	else {
		mesh tree;
		mesh trunk= mesh_primitive_cylinder(radius, position1,position2, 10, 20);
		trunk.color.fill({ 0.45, 0.36, 0.26 });
		tree.push_back(trunk);
		float theta = rand_interval(3.14 / 8, 3.14 / 1.5);
		float length_coeff= rand_interval(1.0/2.0,3.0/4.0);
		float width_coeff = rand_interval(1.0 / 2.0, 3.0 / 4.0);
		
		mesh branch1 = procedural_tree(length*length_coeff, radius * width_coeff, iteration - 1, position2, position2 + vec3{0.0, 0.0, length*length_coeff});
		theta = rand_interval(3.14 / 8, 3.14 / 1.5);
		length_coeff = rand_interval(1 / 2.0f, 3 / 4.0f);
		width_coeff = rand_interval(1 / 2.0f, 3 / 4.0f);
		
		mesh branch2 = procedural_tree(length*length_coeff, radius * width_coeff, iteration - 1, position2, position2 + vec3{std::sin(theta)* length*length_coeff, 0.0, std::cos(theta)* length*length_coeff});
		theta = rand_interval(3.14 / 8, 3.14 / 1.5);
		length_coeff = rand_interval(1 / 2.0f, 3 / 4.0f);
		width_coeff = rand_interval(1 / 2.0f, 3 / 4.0f);
		
		mesh branch3 = procedural_tree(length*length_coeff, radius * width_coeff, iteration - 1, position2, position2 + vec3{std::sin(-theta)* length*length_coeff, 0.0, std::cos(theta)* length*length_coeff});
		theta = rand_interval(3.14 / 8, 3.14 / 1.5);
		length_coeff = rand_interval(1 / 2.0f, 3 / 4.0f);
		width_coeff = rand_interval(1 / 2.0f, 3 / 4.0f);
		
		mesh branch4 = procedural_tree(length*length_coeff, radius * width_coeff, iteration - 1, position2, position2 + vec3{0.0, std::sin(theta)* length*length_coeff, std::cos(theta)* length*length_coeff});
		theta = rand_interval(3.14 / 8, 3.14 / 1.5);
		length_coeff = rand_interval(1 / 2.0f, 3 / 4.0f);
		width_coeff = rand_interval(1 / 2.0f, 3 / 4.0f);
		
		mesh branch5 = procedural_tree(length*length_coeff, radius * width_coeff, iteration - 1, position2, position2 + vec3{0.0, std::sin(-theta)* length*length_coeff, std::cos(theta)* length*length_coeff});
		tree.push_back(branch1);
		tree.push_back(branch2);
		tree.push_back(branch3);
		tree.push_back(branch4);
		tree.push_back(branch5);

		return tree;
		}

		}


mesh create_procedural_tree_mesh(float length, float radius, int iteration, vec3 position) {
	return procedural_tree(length, radius, iteration, position, position+vec3{ 0.0,0.0,length });
}

