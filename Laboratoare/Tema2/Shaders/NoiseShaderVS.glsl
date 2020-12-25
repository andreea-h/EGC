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
uniform float rand;

float randGen(vec3 co)
{
    float a = 12.9898;
    float b = 78.233;
    float c = 48.5453;
    float dt= dot(co, vec3(a, b, c));
    float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

void main()
{
	
	// TODO: send output to fragment shader
	fPosition = v_position;
	fCoord = text_coord;
	fNormal = v_normal;
	fColor = v_color;
	// TODO: compute gl_Position
	// pozitia in clip space a vertexului primit
	float randVal = randGen(v_position);
	vec3 newPos = v_position + v_normal  * sin(time * 10 + v_position.z * 20) * 0.75 * randVal;
	gl_Position = Projection * View * Model * vec4(newPos, 1.0);
}