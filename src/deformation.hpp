#pragma once

#include "cgp/cgp.hpp"
#include "cgp/helper_common_scene/helper_common_scene.hpp"

/** Scene environment that adds the following uniform parameters to be sent to the shader in shaders/deformation/
*/

struct scene_environment_with_shader_deformation : cgp::scene_environment_basic
{
	cgp::vec3 light; // position of the light
	cgp::camera_spherical_coordinates light_view;
	cgp::camera_projection light_projection;
	float t;         // The time is sent to the shader to compute a procedural deformation
	cgp::camera_head camera; // pour l'avion
	cgp::camera_projection projection; //pour l'avion


		// A projection structure (perspective or orthogonal projection)
	//cgp::camera_projection projection;
	// The position/orientation of a camera that can rotates freely around a specific position
	cgp::camera_spherical_coordinates camera_sol;
	scene_environment_with_shader_deformation();

	void update(float t); // function to be called in the scene display - update the time t and the light position

	// brouillard
	// Store position for 5 lights
	std::array<cgp::vec3, 5> spotlight_position;
	// The (r,g,b) color of the 5 lights
	std::array<cgp::vec3, 5> spotlight_color;

	// The characteristic attenuation of the light
	float spotlight_falloff = 0.5f;

	// The characteristic attenuation due to the fog
	float fog_falloff = 0.005f;

	bool plane_view = false;
};

// Send the uniform parameters of the scene with light - Function automatically called when using draw([mesh_drawable], scene_environment_with_multiple_lights);
void opengl_uniform(GLuint shader, scene_environment_with_shader_deformation const& environment);
void opengl_uniform_shadow(GLuint shader, scene_environment_with_shader_deformation const& current_scene);


// Function to call in your display_gui to add the sliders modifying the spotlight and fog falloff
void display_gui_falloff(scene_environment_with_shader_deformation& environment);

struct shadow_map_structure
{
	GLuint shader_shadow_map;    // Shader used to compute the shadow - depth map from the light viewpoint
	GLuint shader_shadow_render; // Shader used to render the 3D shapes with shadows once the shadow map is computed

	GLuint fbo;                // FBO used to render the shadow map as a texture
	GLuint texture_shadow_map; // Texture of the shadow map
	int width_shadow_map;
	int height_shadow_map;


	void initialize(); // Initialize shaders and FBO - must be called in the initialization step before any rendering
	void start_first_pass_shadow_rendering();  // Render objects in the shadow map after this line using draw_shadow_map()
	void end_first_pass_shadow_rendering(int window_width, int window_height);    // Render objects receiving shadows after this line using draw_with_shadow()

	void draw_shadow_map(cgp::mesh_drawable const& drawable, scene_environment_with_shader_deformation const& scene);
	void draw_with_shadow(cgp::mesh_drawable const& drawable, scene_environment_with_shader_deformation const& current_scene);
};
void compute_light_position(float t, scene_environment_with_shader_deformation& environment);
struct light_shape_drawable
{
	cgp::mesh_drawable spotlight_sphere;              // a mesh_drawable of a sphere
	void initialize(GLuint shader_multiple_lights);   // Initialise the spotlight_sphere to a sphere
};
void draw(light_shape_drawable lights, scene_environment_with_shader_deformation const& environment);
