#version 330

// get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// uniform propertiesww
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int OnPowerUps; 
uniform float SpeedRunTime;
uniform int falling;

const int PowerUpsSensitivity = 10;
const float onFallingColorChange = 0.5;
const float PI = 3.14159f;
const float A = 0.05f;
const float f = 10.0f;


// output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture_coord;
out vec3 frag_color;

vec3 apply_distort(vec3 pos)
{
    // sine wave:        y(t) = A * sin(w * t), phase is 0
    // period:           T = 1 / f, where f is frequency
    // angular velocity: w = 2 * PI / T
    
    float T = 1 / f;
    float w = 2 * PI / T;
    float xDistort = A * sin(w * SpeedRunTime * v_position.x);
    float yDistort = A * sin(w * SpeedRunTime * v_position.y);
    float zDistort = A * sin(w * SpeedRunTime * v_position.z);
    return pos + vec3(xDistort, yDistort, zDistort);
}

void main()
{
	// send output to fragment shader
    vec3 pos = v_position;
	frag_position = (Model * vec4(v_position, 1)).xyz;
	frag_normal = normalize(mat3(Model) * normalize(v_normal));
    frag_texture_coord = v_texture_coord;
    frag_color = v_normal;
    if(falling == 1) {
        frag_color -= vec3(onFallingColorChange); 
    } else if(OnPowerUps == 1) {
        pos = apply_distort(pos);
		frag_color = v_normal + sin(PowerUpsSensitivity * SpeedRunTime);
	}
	
	// compute gl_Position
	gl_Position = Projection * View * Model * vec4(pos, 1.0);

}
