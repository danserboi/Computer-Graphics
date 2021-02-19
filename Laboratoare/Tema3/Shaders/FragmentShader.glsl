#version 330

uniform sampler2D texture_1;

// get values from fragment shader
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture_coord;
in vec3 frag_color;

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 color1 = texture2D(texture_1, frag_texture_coord);
	out_color =  mix(color1, vec4(frag_color, 1), 0.5f);

	if (out_color.a < 0.5f) {
		discard;
	}

}