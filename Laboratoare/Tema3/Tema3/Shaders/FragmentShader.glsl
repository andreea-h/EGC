#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int usesTexture2;

in vec2 texcoord;
in float light;

layout(location = 0) out vec4 out_color;
uniform float time;

uniform vec3 light_position;
uniform vec3 eye_position;

uniform vec3 spotLightDirection;
uniform vec3 spotLightPosition;

uniform float kd;
uniform float ks;
uniform int shininess;

uniform float cut_off;

uniform vec3 object_color;


void main()
{
	out_color = texture2D(texture_1, texcoord);
	//out_color *= light;
	if (out_color.a < 0.5) {
		discard;
	}
}