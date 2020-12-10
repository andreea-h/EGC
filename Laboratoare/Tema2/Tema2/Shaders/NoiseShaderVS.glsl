#version 330

// atributele de intrare pentru VertexShader
// TODO: get vertex attributes from each location
layout (location =0) in vec3 v_position;
layout (location =1) in vec3 v_normal;  
layout (location =2) in vec3 text_coord;
layout (location =3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// TODO: output values to fragment shader
out vec3 fNormal;
out vec3 fColor;
out vec3 fPosition;
out vec3 fCoord;

uniform float time;

void main()
{
	// TODO: send output to fragment shader
	fPosition = v_position;
	fCoord = text_coord;
	fNormal = v_normal;
	fColor = v_color;

    vec3 newPos = vec3(v_position.x + sin(v_position.y * 20 + 10 * time) * 0.1, v_position.y, v_position.z);

	// TODO: compute gl_Position
	// pozitia in clip space a vertexului primit
	gl_Position = Projection * View * Model * vec4(newPos, 1.0);
}