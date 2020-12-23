#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int usesTexture2;

in vec2 texcoord;

layout(location = 0) out vec4 out_color;
uniform float time;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	out_color = texture2D(texture_1, texcoord);
	if (out_color.a < 0.5) 
		discard;
	
}

