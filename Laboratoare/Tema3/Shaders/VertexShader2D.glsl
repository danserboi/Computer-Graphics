#version 330

// get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// uniform properties
uniform mat4 Model;
uniform vec3 Color;

// output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture_coord;
out vec3 frag_color;

void main()
{
	// send output to fragment shader
	frag_position = v_position;
	frag_normal	= v_normal;
	frag_texture_coord = v_texture_coord;
	frag_color = Color;

	// compute gl_Position
	gl_Position = Model * vec4(v_position, 1.0);
}
