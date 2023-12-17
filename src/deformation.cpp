#include "deformation.hpp"

using namespace cgp;

void opengl_uniform(GLuint shader, scene_environment_with_shader_deformation const& environment)
{
	// Send the default uniforms from the basic scene_environment
	opengl_uniform(shader, scene_environment_basic(environment));

	// Send the light and time as uniforms 
	opengl_uniform(shader, "projection", environment.projection.matrix());
	opengl_uniform(shader, "view", environment.camera.matrix_view());
	opengl_uniform(shader, "light", environment.light, false);
	opengl_uniform(shader, "time", environment.t, false);	// Note: the 3rd parameter is set to false so that there is no warning if the environment is used with a shader that doesn't expect light and time uniform variable.

	opengl_uniform(shader, "projection", environment.projection.matrix()); // pour l'avion
	opengl_uniform(shader, "view", environment.camera.matrix_view());// pour l'avion

	//brouillard
	// Send the position and color of sphere as uniforms
	/** Note: Here we use the raw OpenGL call to glUniform3fv allowing us to pass a vector of data (here an array of 5 positions and 5 colors) */
	int const N_spotlight = environment.spotlight_color.size();
	GLint const location_color = glGetUniformLocation(shader, "spotlight_color");
	GLint const location_position = glGetUniformLocation(shader, "spotlight_position");
	glUniform3fv(location_color, N_spotlight, ptr(environment.spotlight_color[0]));
	glUniform3fv(location_position, N_spotlight, ptr(environment.spotlight_position[0]));

	// Send the falloff values as uniforms
	opengl_uniform(shader, "spotlight_falloff", environment.spotlight_falloff);
	opengl_uniform(shader, "fog_falloff", environment.fog_falloff);
}
void opengl_uniform_shadow(GLuint shader, scene_environment_with_shader_deformation const& environment)
{
	opengl_uniform(shader, "projection", environment.projection.matrix());
	opengl_uniform(shader, "view", environment.camera.matrix_view());
	opengl_uniform(shader, "light", environment.light_view.matrix_view());
	opengl_uniform(shader, "projection_light", environment.light_projection.matrix());
}

void scene_environment_with_shader_deformation::update(float current_time)
{
	t = current_time;          // set the current time
	light = camera.position(); // replace the light at the camera position
	//if (plane_view)
	//	projection = camera_projection::perspective(50.0f * Pi / 180, 1.0f, 0.01f, 50.0f); //pour l'avion
	//else
		projection = camera_projection::perspective(60.0f * Pi / 180, 1.0f, 0.01f, 500.0f); //pour le sol
}


void display_gui_falloff(scene_environment_with_shader_deformation& environment)
{
	ImGui::SliderFloat("Fog falloff intensity", &environment.fog_falloff, 0, 0.01f, "%0.5f", 2.0f);	

}

void shadow_map_structure::initialize()
{
	// Load the shaders used to compute and render the shadow map
	shader_shadow_render = opengl_load_shader("shaders/shadow_map/use_shadows/vert.glsl", "shaders/shadow_map/use_shadows/frag.glsl");
	shader_shadow_map = opengl_load_shader("shaders/shadow_map/generate_depth_map/vert.glsl", "shaders/shadow_map/generate_depth_map/frag.glsl");

	// Size of the shadow map texture
	width_shadow_map = 2048;
	height_shadow_map = 2048;

	// Initialize texture
	opengl_check;
	glGenTextures(1, &texture_shadow_map);
	glBindTexture(GL_TEXTURE_2D, texture_shadow_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width_shadow_map, height_shadow_map, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Initialize FBO, and attach the texture
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_shadow_map, 0);

	// Indicate that no color will be rendered
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Clear current FBO to avoid side effects
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	opengl_check;


}
scene_environment_with_shader_deformation::scene_environment_with_shader_deformation()
{
	background_color = { 1,1,1 };
	//camera.look_at({ 7.0f, 4.0f, 7.0f }, { 0,0,0 });
	projection = camera_projection::perspective(50.0f * Pi / 180, 1.0f, 0.1f, 500.0f);
	light = { 1.0f,6.0f,2.0f };
	light_view.look_at(light, { 0,0,0 });
	light_projection = camera_projection::orthographic(-10, 10, -10, 10, 0, 30); // orthographic projection for simplicity
}
void shadow_map_structure::start_first_pass_shadow_rendering()
{
	// Set output to FBO
	glViewport(0, 0, width_shadow_map, height_shadow_map);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); opengl_check;
	glClear(GL_DEPTH_BUFFER_BIT); opengl_check;

}
void shadow_map_structure::end_first_pass_shadow_rendering(int window_width, int window_height)
{
	// Clear previous FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0); opengl_check;

	// Set to standard screen output
	glViewport(0, 0, window_width, window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); opengl_check;
}

void shadow_map_structure::draw_with_shadow(mesh_drawable const& drawable, scene_environment_with_shader_deformation const& scene)
{
	// Setup shader
	assert_cgp(drawable.texture != 0, "Try to draw mesh_drawable without texture");

	// Force the shader used to display the shadow map
	glUseProgram(shader_shadow_render); opengl_check;

	// Send uniforms for this shader
	opengl_uniform_shadow(shader_shadow_render, scene);
	opengl_uniform(shader_shadow_render, drawable.shading);
	opengl_uniform(shader_shadow_render, "model", drawable.transform.matrix());

	// Set texture of the shape at index 0
	glActiveTexture(GL_TEXTURE0); opengl_check;
	glBindTexture(GL_TEXTURE_2D, drawable.texture); opengl_check;
	opengl_uniform(shader_shadow_render, "image_texture", 0);  opengl_check;

	// Set shadow map texture at index 1
	glActiveTexture(GL_TEXTURE1); opengl_check;
	glBindTexture(GL_TEXTURE_2D, texture_shadow_map); opengl_check;
	opengl_uniform(shader_shadow_render, "shadow_texture", 1);  opengl_check;

	// Call draw function
	assert_cgp(drawable.number_triangles > 0, "Try to draw mesh_drawable with 0 triangles"); opengl_check;
	glBindVertexArray(drawable.vao);   opengl_check;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.vbo.at("index")); opengl_check;
	glDrawElements(GL_TRIANGLES, GLsizei(drawable.number_triangles * 3), GL_UNSIGNED_INT, nullptr); opengl_check;

	// Clean buffers
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void shadow_map_structure::draw_shadow_map(mesh_drawable const& drawable, scene_environment_with_shader_deformation const& current_scene)
{

	GLuint shader = shader_shadow_map;
	glUseProgram(shader); opengl_check;

	// Send uniforms for this shader
	opengl_uniform(shader, "projection", current_scene.light_projection.matrix());
	opengl_uniform(shader, "light", current_scene.light_view.matrix_view());
	opengl_uniform(shader, "model", drawable.transform.matrix());

	// Call draw function
	assert_cgp(drawable.number_triangles > 0, "Try to draw mesh_drawable with 0 triangles"); opengl_check;
	glBindVertexArray(drawable.vao);   opengl_check;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.vbo.at("index")); opengl_check;
	glDrawElements(GL_TRIANGLES, GLsizei(drawable.number_triangles * 3), GL_UNSIGNED_INT, nullptr); opengl_check;

	// Clean buffers
	glBindVertexArray(0);
}
void compute_light_position(float t, scene_environment_with_shader_deformation& environment)
{
	// Adapt the position and color of the spotlight procedurally as a function of t

	environment.spotlight_color[0] = { 1.0f, 0.0f, 1.0f };
	environment.spotlight_position[0] = {10+ std::cos(t),15+ std::sin(t), 5 + 0.2 * std::cos(3 * t) };

	environment.spotlight_color[1] = { 0.0f, 1.0f, 1.0f };
	environment.spotlight_position[1] = { 40+std::cos(0.5 * t + Pi / 2), 10+std::sin(0.5 * t + Pi / 2), 5 + 0.2 * std::cos(2 * t) };

	environment.spotlight_position[2] = { 20,0,5.05f };
	environment.spotlight_color[2] = 2 * (std::cos(t) + 1.0f) / 2.0 * vec3(1, 1, 1);

	environment.spotlight_position[3] = {-10+ std::cos(0.5 * t + Pi / 2),-10, 5 + std::cos(2 * t) };
	environment.spotlight_color[3] = vec3((std::cos(t) + 1) / 2, std::sin(t), 0.2 * std::cos(3 * t));

	environment.spotlight_position[4] = { 40,40,5 };
	environment.spotlight_color[4] = { 1.0f, 0.9f, 0.5f };


}
void light_shape_drawable::initialize(GLuint shader_multiple_lights)
{
	float const radius_sphere = 0.05f; // Adapt the radius of the spotlight as you wish
	mesh spotlight_sphere_mesh = mesh_primitive_sphere(radius_sphere); // Initialise the spotlight as a sphere
	spotlight_sphere_mesh.flip_connectivity();                 // Flip the normal as the sphere is illuminated from its inside.
	spotlight_sphere.initialize(spotlight_sphere_mesh, "Spotlight Sphere", shader_multiple_lights); // Convert the mesh to a mesh_drawable with the correct shader
}

void draw(light_shape_drawable lights, scene_environment_with_shader_deformation const& environment)
{
	// Display all the spotlight at their position and with their respective color.
	for (size_t k = 0; k < environment.spotlight_position.size(); ++k)
	{
		lights.spotlight_sphere.transform.translation = environment.spotlight_position[k];
		lights.spotlight_sphere.shading.color = environment.spotlight_color[k];
		draw(lights.spotlight_sphere, environment);
	}
}
