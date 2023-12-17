#include "scene.hpp"
#include "deformation/deformation.hpp" 
#include "cgp/math/math.hpp"
#include "island.hpp"
#include "volcano.hpp"
#include "palm.hpp"
#include "submarine.hpp"
#include "procedural_tree.hpp"
#include "town.hpp"
#include "perlin_rock.hpp"

/** This file contains the custom implementation associated the scene
*  The (*) indicates elements that are specific to the use of the shader deformation and that should be handled if you want to use this effect in another scene. */

using namespace cgp;
bool rotat = true;

 //The main function implementing the Flying Mode
void scene_structure::update_camera() 
{
	inputs_keyboard_parameters const& keyboard = inputs.keyboard;
	camera_spherical_coordinates camera_sol = environment.camera_sol;


		camera_head& camera = environment.camera;
		if (rotat == true) {
			camera.manipulator_rotate_roll_pitch_yaw(0, -30, 0);
			camera.position_camera = { 0,0,5 };
		}
		rotat = false;

		// The camera moves forward all the time
		//   We consider in this example a constant velocity, so the displacement is: velocity * dt * front-camera-vector
		bool const dans_la_zone = camera.position_camera.x < 40 && camera.position_camera.y < 20 &&
			camera.position_camera.x > -2 && camera.position_camera.y > -20;
		if (gui.rescue) {

			quad.texture = opengl_load_texture_image("assets/text_survival.png");
			draw(quad, environment_ortho);
			if (!dans_la_zone) {
				camera.position_camera += (int(camera.position_camera.x <= -2) - int(camera.position_camera.x >= 40)) * vec3 { 0.5, 0, 0 };
				camera.position_camera += (int(camera.position_camera.y <= -20) - int(camera.position_camera.y >= 20)) * vec3 { 0, 0.5, 0 };
			}
			if (camera.position_camera.z > 10)
				camera.position_camera -= vec3{ 0,0,1 };

		}
		if (gui.plane_view) {
			quad.texture = opengl_load_texture_image("assets/text.png");
			float const dt = timer.update();
			vec3 const forward_displacement = gui.speed * 2.0f * dt * camera.front();
			//camera.temp += forward_displacement;
			camera.position_camera += forward_displacement;

			// The camera rotates if we press on the arrow keys
			//  The rotation is only applied to the roll and pitch degrees of freedom.
			float const pitch = 0.5f; // speed of the pitch
			float const roll = 0.7f; // speed of the roll
			float const yaw = 0.5f;
			if (keyboard.up)
				camera.manipulator_rotate_roll_pitch_yaw(0, pitch * dt, 0);
			if (keyboard.down)
				camera.manipulator_rotate_roll_pitch_yaw(0, -pitch * dt, 0);

			if (keyboard.right)
				camera.manipulator_rotate_roll_pitch_yaw(0, 0, -yaw * dt);
			if (keyboard.left)
				camera.manipulator_rotate_roll_pitch_yaw(0, 0, yaw * dt);
			//
			if (keyboard.touched_z)
				camera.position_camera += forward_displacement;
			if (keyboard.touched_q)
				camera.manipulator_rotate_roll_pitch_yaw(roll * dt, 0, 0);
			if (keyboard.touched_d)
				camera.manipulator_rotate_roll_pitch_yaw(-roll * dt, 0, 0);
		}
		else {
			
			
				if (keyboard.up)
					camera.position_camera += camera.front() / 2;
				//camera.manipulator_translate_in_plane({ 0,-0.5 });
				if (keyboard.down)
					camera.position_camera -= camera.front() / 3;
				//camera.manipulator_translate_in_plane({ 0,0.5 });
				if (keyboard.left)
					camera.position_camera -= camera.right() / 3;
				if (keyboard.right)
					camera.position_camera += camera.right() / 3;
				if (keyboard.touched_q)
					camera.manipulator_rotate_roll_pitch_yaw(0, 0, 0.1);
				if (keyboard.touched_d)
					camera.manipulator_rotate_roll_pitch_yaw(0, 0, -0.1);
				//if (keyboard.touched_z)
					//camera.manipulator_rotate_roll_pitch_yaw(0, 0.1, 0);
				if (keyboard.touched_s)
					camera.manipulator_rotate_roll_pitch_yaw(0, -0.1, 0);
			

			vec2 const& p1 = inputs.mouse.position.current;
			vec2 const& p0 = inputs.mouse.position.previous;
			bool const event_valid = !inputs.mouse.on_gui;
			bool const click_left = inputs.mouse.click.left;
			bool const click_right = inputs.mouse.click.right;



			if (event_valid) { // If the mouse cursor is not on the ImGui area
				if (click_left) {
					camera.manipulator_rotate_trackball(p0, p1);
				}
				if (click_right)        // Move the camera closer/further with respect to its center
					//camera.position_camera = -camera.front();
					camera.manipulator_scale_distance_to_center((p1 - p0).y);
			}
			if (keyboard.ctrl)
				camera.position_camera += vec3{0,0,-0.3};
			if(keyboard.shift)
				camera.position_camera += vec3{ 0,0,0.3 };

			if (camera.position_camera.z < 5)
				camera.position_camera += camera.up();

		}
		
	
}
void dynamic_update(mesh& mesh_shape, mesh_drawable& drawable, float t)
{
	// 1- Update the mesh as your wish
	int N = mesh_shape.position.size();
	for (int k = 0; k < N; ++k) {
		// Changing the positions
		vec3& p = mesh_shape.position[k];
		p.z = 0.05f * std::cos(4 * Pi * p.x - t);

		// The colors can be changed too
		vec3& c = mesh_shape.color[k];
		c.x = std::abs(std::cos(0));
	}

	// 2- Don't forget to update the shape normals after modifying the position
	mesh_shape.compute_normal();

	// 3- Update the buffers of the corresponding mesh_drawable
	drawable.update_position(mesh_shape.position);
	drawable.update_color(mesh_shape.color);
	drawable.update_normal(mesh_shape.normal);

	// Note: Using mesh_drawable.update() calls glBufferSubData
	//  This is more efficient that re-allocating a new mesh_drawable
}
void scene_structure::initialize()
{
	// Specific Shader (*)
	//GLuint const shader_deformation = opengl_load_shader("shaders/deformation/vert.glsl", "shaders/deformation/frag.glsl");
	

	//second environment
	
	environment_ortho.projection = camera_projection::orthographic(-1, 1, -1, 1, -1, 1);
	//environment_ortho.camera.distance_to_center = 2.5f;
	environment_ortho.light = { 0,0,1 };
	//environment_ortho.camera.look_at({ 0, 0, 0.5f }, { 0,0,0 });
	
	//brouillard

	environment.background_color = { 0.7f, 0.7f, 0.8f };

	GLuint const shader_lights = opengl_load_shader("shaders/mesh_lights/vert.glsl", "shaders/mesh_lights/frag.glsl");
	mesh_drawable::default_shader = shader_lights;   // set this shader as the default one for all new shapes declared after this line 
	//submarine
	mesh submarine_mesh = create_submarine_mesh();
	big_submarine.initialize(submarine_mesh, "big_submarine");
	big_submarine.transform.scaling = 2.0f;
	big_submarine.transform.translation = { -10.0f,-10.0f,1.0f };
	
	submarine.initialize(submarine_mesh, "big_submarine");
	submarine.transform.scaling = 0.3f;


		
													 // The shadow map paramters must be initialized before their use
	shadow_map.initialize();
	light_drawable.initialize(shader_lights);


	// Create the Shapes
	// ***************************************** //

	// The standard frame
	global_frame.initialize(mesh_primitive_frame(), "Frame");

	// the sea
	sea_mesh = create_terrain_mesh(20.f);
	sea.initialize(sea_mesh, "sea");

	//quad
	quad.initialize(mesh_primitive_quadrangle({ -0.5f,0.7f,0 }, { 0.5f,0.7f,0 }, { 0.5f,1.0f,0 }, { -0.5f,1.0f,0 }), "Quad");
	quad.shading.color = { 0.7,0.1,0.1 };
	quad.texture = opengl_load_texture_image("assets/text.png");
	// the island

	mesh island_mesh = create_island_mesh(20.0,true);
	//mesh const island_mesh_plat = create_island_mesh(20.0, false);
	island_mesh_ref = create_island_mesh(20.0, false);


	mesh const mountain_mesh = create_mountain_mesh(100, 5);

	int N_island = std::sqrt(island_mesh.position.size());;
	for (int ku = 0; ku < N_island; ++ku) {
		for (int kv = 0; kv < N_island; ++kv) {

			// Compute local parametric coordinates (u,v) \in [0,1]
			const float u = ku / (N_island - 1.0f);
			const float v = kv / (N_island - 1.0f);

			int const idx = ku * N_island + kv;

			// Compute the Perlin noise
			
			float const noise = perlin_noise(u , v, perlin_rock_parameters2);

			// use also the noise as color value
			island_mesh.color[idx] = 0.3f * vec3{ 0.96, 0.89, 0.75 } + 0.7f * noise * vec3{ 0.96,0.89,0.75 };
		}
	}
	island.initialize(island_mesh, "Island");
	island3.initialize(island_mesh_ref, "Island_plat");	
	update_terrain(island_mesh_ref, island3, perlin_rock_parameters2, 0);

	mountain.initialize(mountain_mesh, "Mountain");
	island.shading.color = { 0.96,0.89,0.75 };
	island3.shading.color = { 0.96,0.89,0.75 };
	mountain.shading.color = { 0.1,0.8,0.3 };
	island.shading.phong.specular = 0.0f; // non-specular terrain material
	island3.shading.phong.specular = 0.0f; // non-specular terrain material
	mountain.shading.phong.specular = 0.0f;
	GLuint const sand_texture = opengl_load_texture_image("assets/sable2.jpg",
		GL_REPEAT, GL_REPEAT);
	island.texture = sand_texture;
	island3.texture = sand_texture;
	GLuint const grass_texture = opengl_load_texture_image("assets/grass.jpg",
		GL_REPEAT, GL_REPEAT);
	mountain.texture = grass_texture;


	//the volcano

	GLuint const volcano_shader = opengl_load_shader("shaders/multitexture/vert.glsl", "shaders/multitexture/frag.glsl");

	// Set the two textures and the specific shader

	mesh const volcano_mesh = create_volcano_mesh(N_volcano, volcano_length, volcano_height, volcano_radius);
	volcano1.initialize(volcano_mesh, "Volcano1");
	volcano2.initialize(volcano_mesh, "Volcano2");

	//volcano.shading.color = { 0.43,0.2,0 };
	volcano1.shading.phong.specular = 0.0f; // non-specular terrain material
	volcano2.shading.phong.specular = 0.0f; // non-specular terrain material

	volcano1.transform.translation = { -island_radius / 3,-island_radius / 3,0.0f };
	volcano2.transform.translation = { -island_radius / 3,-island_radius / 3,0.0f };

	volcano1.texture = opengl_load_texture_image("assets/roche.jpg");
	volcano1.texture_2 = opengl_load_texture_image("assets/lava.jpg");
	volcano2.texture = opengl_load_texture_image("assets/roche.jpg");
	volcano2.texture_2 = opengl_load_texture_image("assets/lava.jpg");

	volcano2.shader = volcano_shader; 
	lava.initialize(mesh_primitive_cylinder(0.5f, vec3{ 0.0,0.0,0.0 }, vec3{ 0.0,0.0,volcano_height / 1.5 }, 10, 20, true));
	GLuint const lava_texture = opengl_load_texture_image("assets/lava.jpg",
		GL_REPEAT, GL_REPEAT);
	lava.texture = lava_texture;
	lava.transform.translation = { -island_radius / 3,-island_radius / 3 ,0.0f };


	//volcano eruption
	float const sphere_radius = 0.05f;
	lava_particle.initialize(mesh_primitive_sphere(sphere_radius), "sphere");
	lava_particle.shading.color = { 1.0f,0.0f,0.0f };
	lava_timer.event_period = 10.0f;




	//drone
	vec3 drone_center = vec3{ 0.0f,0.0f,5.0f };

	drone_cylinder1.initialize(mesh_primitive_cylinder(0.1f, drone_center + vec3{ -0.1f,0.1f,2.0f }, drone_center + vec3{ -0.1f,0.1f,2.01f }), "cylinder1");
	drone_cylinder2.initialize(mesh_primitive_cylinder(0.1f, drone_center + vec3{ -0.1f,-0.1f,2.0f }, drone_center + vec3{ -0.1f,-0.1f,2.01f }), "cylinder2");
	drone_cylinder3.initialize(mesh_primitive_cylinder(0.1f, drone_center + vec3{ 0.1f,0.1f,2.0f }, drone_center + vec3{ 0.1f,0.1f,2.01f }), "cylinder3");
	drone_cylinder4.initialize(mesh_primitive_cylinder(0.1f, drone_center + vec3{ 0.1f,-0.1f,2.0f }, drone_center + vec3{ 0.1f,-0.1f,2.01f }), "cylinder4");

	drone_cylinder1.shading.color = { 1.0,0.0,0.0 };
	drone_cylinder2.shading.color = { 1.0,0.0,0.0 };
	drone_cylinder3.shading.color = { 1.0,0.0,0.0 };
	drone_cylinder4.shading.color = { 1.0,0.0,0.0 };


	drone_helix1.initialize(mesh_primitive_ellipsoid(vec3{ 0.1f,0.01f,0.02f }, drone_center + vec3{ -0.1f,0.1f,2.0f }), "helix1");
	drone_helix2.initialize(mesh_primitive_ellipsoid(vec3{ 0.1f,0.01f,0.02f }, drone_center + vec3{ -0.1f,-0.1f,2.0f }), "helix2");
	drone_helix3.initialize(mesh_primitive_ellipsoid(vec3{ 0.1f,0.01f,0.02f }, drone_center + vec3{ 0.1f,0.1f,2.0f }), "helix3");
	drone_helix4.initialize(mesh_primitive_ellipsoid(vec3{ 0.1f,0.01f,0.02f }, drone_center + vec3{ 0.1f,-0.1f,2.0f }), "helix4");

	drone_helix1.shading.color = { 0.0,0.0,0.0 };
	drone_helix2.shading.color = { 0.0,0.0,0.0 };
	drone_helix3.shading.color = { 0.0,0.0,0.0 };
	drone_helix4.shading.color = { 0.0,0.0,0.0 };



	drone.add(drone_cylinder1);
	drone.add(drone_cylinder2, "cylinder1");
	drone.add(drone_cylinder3, "cylinder1");
	drone.add(drone_cylinder4, "cylinder1");
	drone.add(drone_helix1, "cylinder1");
	drone.add(drone_helix2, "cylinder2");
	drone.add(drone_helix3, "cylinder3");
	drone.add(drone_helix4, "cylinder4");


	procedural_tree.initialize(create_procedural_tree_mesh(procedural_tree_length, procedural_tree_width, 5, vec3{ island_radius / 3,island_radius / 3,0.0f }));
	trees = new double* [nb_trees];
	for (int i = 0; i < nb_trees; i++) {
		double* one_tree = new double[2];
		one_tree[0] = rand_interval(10, 20);
		one_tree[1] = rand_interval(-18, 15);
		trees[i] = one_tree;

	}
	building.initialize(town());
	building.transform.translation = { 35, 8, 3 };

	//flags
	support.initialize(mesh_primitive_cylinder(0.05, { 1,3.5,0 }, {1,3.5,6}), "support");
	int N = 100;
	mesh_shape = mesh_primitive_grid({ 0,0,0 }, { 1,0,0 }, { 1,1,0 }, { 0,1,0 }, N, N);
	drawable.initialize(mesh_shape, "Dynamic surface");
	drawable.texture = opengl_load_texture_image("assets/polytechnique_2.jpg");



}








void scene_structure::display()
{
	timer.update();
	float t = timer.t;
	// The standard frame
	if (gui.display_frame)
		draw(global_frame, environment);

	if (gui.animated_light) {
		environment.light = { 6 * cos(0.5 * t), 6.0f , 6 * sin(0.5 * t) };
		environment.light_view.look_at(environment.light, { 0,0,0 });
	}


	// Displaying the shape deformed by the shader
	environment.update(t);   // update the environment with the new time and light position 
	

		island.transform.translation = { 0,0,0 };
		draw(island, environment); // display the island

	environment.light = environment.camera.position();

	perlin_rock_parameters2.persistency = 0.6f;
	perlin_rock_parameters2.frequency_gain = 2.5f;
	perlin_rock_parameters2.octave = 8;
	perlin_rock_parameters2.terrain_height = 30.0f;
	perlin_rock_parameters2.color1 = { 0.96,0.89,0.75 };
	perlin_rock_parameters2.color2 = { 0.96,0.89,0.75 };


	island3.transform.translation = { 38,0,2 };
	draw(island3, environment); // display the island





	//mountain.transform.translation = { 0,0,10 };
	//draw(mountain, environment);


	volcano1.transform.scaling = 5.0f;
	volcano1.transform.translation = { 45.0f,-10.0,0.0f };
	volcano2.transform.scaling = 5.0f;
	volcano2.transform.translation = { 45.0f,-10.0,0.0f };

	if (gui.brouillard)
		draw(volcano1, environment); // display the volcano
	else {
		draw(volcano2, environment); // display the volcano
		environment.fog_falloff = 0;
	}

	inputs_keyboard_parameters const& keyboard = inputs.keyboard;
	if (keyboard.drs){
	draw(quad, environment_ortho);
	}


	draw(lava, environment);
	//
	//draw(skybox, environment);
	////draw(moon, environment);

	drone["cylinder1"].transform.translation = { 3.0f * std::cos(t),3.0f + 5.0f * std::sin(t),2.0f };
	drone["helix1"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, 20 * timer.t);
	drone["helix2"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, 20 * timer.t);
	drone["helix3"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, 20 * timer.t);
	drone["helix4"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, 20 * timer.t);

	drone.update_local_to_global_coordinates();
	draw(drone, environment);

	big_submarine.transform.translation = { -10.0f,-10.0f,1.0 + std::sin(0.5 * t) };

	draw(big_submarine, environment);

	float dt_submarine = submarines_timer.update();

	bool const new_submarine = submarines_timer.event;
	if (new_submarine == true) {


		vec3 const p0 = { rand_interval(-15.0f,-8.0f),rand_interval(12.0,-5.0f),-0.5f };


		// initial random velocity (x,y) components are uniformly distributed along a circle.
		const float theta = rand_interval(0, 2 * 3.14);

		const vec3 v0 = vec3(4 * std::cos(theta), 4 * std::sin(theta), 8.0);

		submarines.push_back({ p0,v0,theta,-3.14f / 12.0f });
	}
	for (little_submarines& submarine : submarines)
	{
		const float m = 0.01f; // particle mass

		vec3& p = submarine.p;
		vec3& v = submarine.v;
		float theta = submarine.theta;
		float inclinaison = submarine.inclinaison;

		const vec3 f = m * g;

		// numerical integration
		v = v + dt_submarine * f / m;
		p = p + dt_submarine * v;

		inclinaison = inclinaison + 0.1 * dt_submarine;
	}
 


// remove particles that are too low
	for (auto it = submarines.begin(); it != submarines.end(); ) {
		if (it->p.z < -3)
			it = submarines.erase(it);
		if (it != submarines.end())
			++it;
	}
	// display submarines
	for (little_submarines& particle : submarines)
	{

		submarine.transform.scaling = 0.3f;
		submarine.transform.translation = particle.p;
		submarine.transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, particle.inclinaison);
		submarine.transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, particle.theta);
		draw(submarine, environment);

	}

//volcano eruption
	if (gui.eruption) {
		float dt_lava = lava_timer.update();

		bool const new_particle = lava_timer.event;
		if (new_particle == true) {
			for (int i = 0; i < 300; i++) {
				vec3 const p0 = { -island_radius / 3,-island_radius / 3 ,5.0f };

				// Initial random velocity (x,y) components are uniformly distributed along a circle.
				const float theta = rand_interval(0, 2 * Pi);
				const float inclinaison = rand_interval(4, 10);
				const vec3 v0 = vec3(inclinaison * std::cos(theta), inclinaison * std::sin(theta), 7.0f);

				lava_particles.push_back({ p0,v0 });
			}
		}

		// Evolve position of particles
		const vec3 g = { 0.0f,0.0f,-9.81f };
		for (particle_structure& particle : lava_particles)
		{
			const float m = 0.01f; // particle mass

			vec3& p = particle.p;
			vec3& v = particle.v;

			const vec3 F = m * g;

			// Numerical integration
			v = v + dt_lava * F / m;
			p = p + dt_lava * v;
		}

		// To do: 
		//  Add the collision handling/bouncing to the particles here
		//  ...

		// Remove particles that are too low
		for (auto it = lava_particles.begin(); it != lava_particles.end(); ) {
			if (it->p.z < 0)
				it = lava_particles.erase(it);
			if (it != lava_particles.end())
				++it;
		}

		// Display particles
		for (particle_structure& particle : lava_particles)
		{
			lava_particle.transform.translation = particle.p+vec3{45-island_radius/3,-10-island_radius/3,5};
			draw(lava_particle, environment);
		}
	}
	

	for (int i = 0; i < nb_trees; i++) {
		procedural_tree.transform.translation = { trees[i][0],trees[i][1],2.7f };
		draw(procedural_tree, environment);
	}



	perlin_rock_parameters1.persistency = 0.6f;
	perlin_rock_parameters1.frequency_gain = 2.5f;
	perlin_rock_parameters1.octave = 8;
	perlin_rock_parameters1.terrain_height = 30.0f;
	perlin_rock_parameters1.color1 = {0.01, 0.13, 0.3};
	perlin_rock_parameters1.color2 = { 0.28, 0.61, 0.85 };


	update_terrain(sea_mesh, sea, perlin_rock_parameters1, 0.02 * t);
	sea.transform.translation = { 0,0,0 };
	draw(sea, environment);

		// Update the position and color of the lights
	compute_light_position(t, environment);
	draw(light_drawable, environment); // this is a helper function from multiple_lights (display all the spotlights as spheres) (*)-optionnal
	draw(building, environment);

	// Call of the update
	dynamic_update(mesh_shape, drawable, timer.t);

	// Standard Display
	drawable.transform.translation = { 1,3.5,5 };
	drawable.transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 3.14/2);

	draw(drawable, environment);
	draw(support, environment);
}




void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::SliderFloat("Plane Speed", &gui.speed, 0.1f, 5.0f); // Set the camera velocity
	display_gui_falloff(environment); // helper function from multiple_lights (*)-optionnal
	//ImGui::Checkbox("Plane mode", &gui.plane_view);
	ImGui::Checkbox("Play volcano eruption", &gui.eruption);
	ImGui::Checkbox("I am lost, bring me to the island", &gui.rescue);
	ImGui::Checkbox("Brouillard", &gui.brouillard);

	ImGui::Text("On land, use click left, Q and D for camera orientation");
	ImGui::Text("On land, use arrows to move");

	ImGui::Text("Press Z to activate plane DRS");
	ImGui::Text("Use arrows, Q and D for plane mouvement");




}

