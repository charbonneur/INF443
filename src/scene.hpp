#pragma once

/** This file contains the custom function signature and structure associated the scene
*  The (*) indicates elements that are specific to the use of the shader_deformation  */

#include "cgp/cgp.hpp"
#include "deformation/deformation.hpp" 



#include "island.hpp"
#include "volcano.hpp"
#include "palm.hpp"
#include "submarine.hpp"
#include "procedural_tree.hpp"
#include "town.hpp"
#include "perlin_rock.hpp"
#include "mesh_drawable_multitexture/mesh_drawable_multitexture.hpp"
#include "environment_camera_head/environment_camera_head.hpp"

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = true;
	float speed = 1.0f;       // The velocity of the camera displacement
	bool display_wireframe = false;
	bool animated_shapes = true;
	bool animated_light = true;
	bool plane_view = false;
	bool brouillard = false;
	bool camera_origine = true;
	bool rescue = false;
	bool eruption = false;
};

struct particle_structure
{
	cgp::vec3 p; // Position
	cgp::vec3 v; // Speed
};

struct little_submarines
{
	cgp::vec3 p; // Position
	cgp::vec3 v; // Speed
	float theta;
	float inclinaison;
};




// The structure of the custom scene
struct scene_structure {

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	scene_environment_with_shader_deformation environment; // The specific scene environment associated to the deforming shader (*)
	scene_environment_with_shader_deformation environment_ortho;

		// The structure handling the rendering for the shadow map
	shadow_map_structure shadow_map;
	light_shape_drawable light_drawable; // Helper structure used to display the lights as spheres (*)


	cgp::mesh_drawable global_frame;    // The standard global frame
	cgp::mesh_drawable sea;

	cgp::mesh_drawable sea2;
	cgp::mesh sea_mesh;           // The sea that is deformed by the shader
	cgp::mesh sea_mesh2;           // The sea that is deformed by the shader

	float sea_length = 20.0f;
	cgp::mesh island_mesh_ref;
	cgp::mesh_drawable island;
	cgp::mesh_drawable island3;
	cgp::mesh_drawable mountain;
	int N_island = 100;
	float island_radius = 4.0f;
	mesh_drawable_multitexture  volcano1;
	mesh_drawable_multitexture  volcano2;

	int N_volcano = 100;
	float volcano_radius = 0.5f;
	float volcano_height = 3.0f;
	float volcano_length = 4.0f;
	cgp::mesh_drawable lava;
	//sea
	//lumiere


	cgp::mesh_drawable palm_leaf;
	cgp::mesh_drawable palm_trunk;
	float palm_height = 2.0f;
	float palm_radius = 0.1f;
	cgp::mesh_drawable moon;

	cgp::mesh_drawable procedural_tree;

	float procedural_tree_length = 3.0f;
	float procedural_tree_width = 0.3f;
	double** trees;
	int nb_trees = 20;



	// sky
	//cgp::skybox_drawable skybox;
	cgp::mesh_drawable quad;

	cgp::mesh_drawable oak;

	//drone
	cgp::hierarchy_mesh_drawable drone;
	cgp::mesh_drawable drone_cylinder1;
	cgp::mesh_drawable drone_cylinder2;
	cgp::mesh_drawable drone_cylinder3;
	cgp::mesh_drawable drone_cylinder4;
	cgp::mesh_drawable drone_helix1;
	cgp::mesh_drawable drone_helix2;
	cgp::mesh_drawable drone_helix3;
	cgp::mesh_drawable drone_helix4;

	//submarine
	cgp::mesh_drawable submarine;
	cgp::mesh_drawable big_submarine;

	std::vector<little_submarines> submarines; //list where we store the little submarines


	//volcano eruption
	cgp::mesh_drawable lava_particle;
	std::vector<particle_structure> lava_particles; // Storage of all currently active particles

	//perlin rock
	perlin_noise_parameters perlin_rock_parameters1;
	perlin_noise_parameters perlin_rock_parameters2;
	
	//town
	cgp::mesh_drawable building;

	// We keep two elements in memory:
//  - The mesh structure whose buffers can be modified at every frame
//  - The mesh_drawable structure (as usual) that is displayed and dynamically updated from the mesh.
	cgp::mesh mesh_shape;
	cgp::mesh_drawable drawable;
	cgp::mesh_drawable support;

	//timer

	cgp::timer_event_periodic lava_timer; //timer for volcano eruption
	cgp::timer_event_periodic submarines_timer; //timer for littles submarines
	cgp::timer_basic timer; // A basic timer for the procedural deformation of the shape
	gui_parameters gui;     // The standard GUI element storage
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)


	const cgp::vec3 g = { 0.0f,0.0f,-9.81f }; //constant of gravity
	// camera

	void update_camera();

	//cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
	
};




