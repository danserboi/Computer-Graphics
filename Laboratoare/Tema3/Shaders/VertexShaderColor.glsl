#version 330

// get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Color;
uniform int ObjectType;
uniform float Time;


// output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture_coord;
out vec3 frag_color;

const int TimeSensitivity = 2;

void main()
{
	// send output to fragment shader
	frag_position = (Model * vec4(v_position, 1)).xyz;
	frag_normal = normalize(mat3(Model) * normalize(v_normal));
	frag_texture_coord = v_texture_coord;
	frag_color = Color;
    if(ObjectType == 1) {
        frag_color += abs(sin(TimeSensitivity * Time));
    }
	// compute gl_Position
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
