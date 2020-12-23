#version 330
// TODO: get values from fragment shader
in vec3 fNormal;
in vec3 fColor;
in vec3 fPosition;
in vec3 fCoord;

layout(location = 0) out vec4 out_color;

uniform vec3 object_color;
uniform float time;
uniform float rand;

float randGen(vec3 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43.5453;
    highp float dt= dot(co, vec3(a, b, c));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

void main()
{
	// TODO: write pixel out color
	out_color = vec4((sin(time) * 0.5 + 0.5) * object_color, 1.0);
}