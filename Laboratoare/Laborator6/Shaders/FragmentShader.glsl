#version 330

// TODO: get values from fragment shader
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture_coord;
in vec3 frag_color;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: write pixel out color
	out_color = vec4(frag_color, 1);
	// EX 5:
	// out_color = vec4(frag_normal, 1);
	// VertexFormat are in componenta urmatoarele atribute:
	// pozitia vertex-ului glm::vec3 position;		
	// normala vertex-ului glm::vec3 normal
	// coordonatele de textura ale vertex-ului: glm::vec2 text_coord
	// culoarea vertex-ului glm::vec3 color
	// Daca se utilizeaza normala vertexilor pe post de culoare de output in cadrul Fragment Shader-ului
	// vom folosi valorile pt atributul glm::vec3 normal din VertexFormat
	// astfel, cube va avea un alt colorit
	// iar box nu va mai fi complet negru, va avea 3 fete colorate cu rosu, verde si albastru
}