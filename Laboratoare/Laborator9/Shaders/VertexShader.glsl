#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int EarthObject;
uniform float Time;

const float TimeSensitivity = 20.0f;

out vec2 texcoord;

void main()
{
	// TODO : pass v_texture_coord as output to Fragment Shader
	texcoord = v_texture_coord;

	if (EarthObject == 1) {
		texcoord = vec2(texcoord.x - Time / TimeSensitivity, texcoord.y);
	}

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
