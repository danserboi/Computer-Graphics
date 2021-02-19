#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

// light direction vector
vec3 L = normalize(light_position - world_position);
// observer direction vector
vec3 V = normalize(eye_position - world_position);
// median vector
vec3 H = normalize(L + V);
// normal vector
vec3 N = normalize(world_normal);

uniform vec3 material_ke;
uniform vec3 material_ka;
uniform vec3 material_kd;
uniform vec3 material_ks;
uniform vec3 light_source_intensity;
uniform vec3 ambient_light_intensity;
uniform int material_shininess;

uniform int light_mode;
uniform float cut_off_angle;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: define ambient light component
	vec3 ambient_light = ambient_light_intensity * material_ka;

	// TODO: compute diffuse light component
	vec3 diffuse_light = material_kd * light_source_intensity * max(dot(N, L), 0);

	// TODO: compute specular light component
	vec3 specular_light = material_ks * light_source_intensity * pow(max(dot(N, H), 0), material_shininess);

	// TODO: compute light
	vec3 light;
	if (light_mode == 1)
	{
		float cut_off = radians(cut_off_angle);
		float spot_light = dot(-L, light_direction);
		float spot_light_limit = cos(cut_off);
		if (spot_light > spot_light_limit)
		{	 
			float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);
			light = ambient_light + light_att_factor * (diffuse_light + specular_light);
		} else {
			light = ambient_light;
		}
	} else {
		light = ambient_light + diffuse_light + specular_light;
	}

	// TODO: write pixel out color
	out_color = vec4(light, 1);
}