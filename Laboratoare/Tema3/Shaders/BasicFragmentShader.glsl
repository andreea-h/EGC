#version 330
in vec3 fNormal;
in vec3 fColor;
in vec3 fPosition;
in vec2 fCoord;

uniform sampler2D textures;
uniform int usesTexture;

layout(location = 0) out vec4 out_color;

uniform vec3 object_color;


void main()
{
	// TODO: write pixel out color
	out_color = vec4(object_color, 1.0);
	if (usesTexture == 1) {
		out_color = texture2D(textures, fCoord);
	}
	
}