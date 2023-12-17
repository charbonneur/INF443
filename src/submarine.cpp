#include "submarine.hpp"

using namespace cgp;

mesh create_submarine_mesh() {
	mesh main = mesh_primitive_ellipsoid(vec3{ 2.0f,0.5f,0.5f }, { 0,0,0 });
	mesh periscope_base = mesh_primitive_cylinder(0.1f, { 0,0,0.4f }, { 0,0,1.0f }, 40,40,true);
	mesh periscope_head=mesh_primitive_cylinder(0.1f, { -0.05,0,0.9f }, { 0.5,0,0.9f });
	mesh tail = mesh_primitive_sphere(0.3f, { -2.3f,0,0 });
	mesh tail2 = mesh_primitive_cube({ -2.3f,0,0 }, 0.5f);
	mesh glass = mesh_primitive_disc(0.1f, { 0.5,0,0.9f }, { 1,0,0 });
	mesh submarine = main;
	
	submarine.push_back(periscope_base);
	submarine.push_back(periscope_head);
	submarine.push_back(tail);
	submarine.push_back(tail2);


	submarine.color.fill({ 0.93,0.73,0.33 });
	glass.color.fill({ 0.02,0.9,0.75 });
	submarine.push_back(glass);
	submarine.fill_empty_field();
	return submarine;

}