#version 330

// TODO: get values from fragment shader
in vec3 fNormal;
in vec3 fColor;
in vec3 fPosition;
in vec3 fCoord;

layout(location = 0) out vec4 out_color;

uniform float time;

void main()
{
	// TODO: write pixel out color
	out_color = vec4(fNormal * sin(time) + 0.5f, 1); //variaza culoarea dupa functia sin

}