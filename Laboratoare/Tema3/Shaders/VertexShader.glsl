#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform sampler2D texture_1;
uniform sampler2D texture_2;


uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 texcoord;
uniform float time;

out float light;

uniform vec3 light_position;
uniform vec3 eye_position;

uniform vec3 spotLightDirection;
uniform vec3 spotLightPosition;

uniform float kd;
uniform float ks;
uniform int shininess;

uniform float cut_off;

layout(location = 0) out vec4 out_color;


void main()
{
	vec3 world_normal = normalize((Model * vec4(v_normal, 0.0)).xyz);
	vec3 world_position = (Model * vec4(v_position, 1)).xyz;

	
	world_position = (Model * vec4(v_position, 1)).xyz;
	world_normal = normalize(mat3(Model) * v_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(L, normalize(world_normal)));

	vec3 LS = normalize(spotLightPosition - world_position);
	vec3 VS = normalize(eye_position - world_position);
	vec3 HS = normalize(LS + VS);
	vec3 RS = normalize(reflect(LS, normalize(world_normal)));

	float ambient_light = 0.25f;

	float diffuse_light = kd * max(dot(normalize(world_normal), L), 0.f);

	// Compute specular light component
	float specular_light = 0.f;

	if (diffuse_light > 0.f)
	{
		// specular_light = material_ks * pow(max(dot(V, R), 0.f), material_shininess);
		specular_light =ks * pow(max(dot(normalize(world_normal), H), 0.f), shininess);
	}

	// Compute diffuse light component
	float head_diffuse_light = kd * max(dot(normalize(world_normal), LS), 0.f);

	// Compute specular light component
	float head_specular_light = 0.f;

	if (head_diffuse_light > 0.f)
	{
		// head_specular_light = material_ks * pow(max(dot(VS, RS), 0.f), material_shininess);
		head_specular_light = ks * pow(max(dot(normalize(world_normal), HS), 0.f), shininess);
	}

	// Compute light
	float head_light = 0.f;

	float spot_light = dot(-LS, spotLightDirection);
	float spot_light_limit = cos(cut_off);
	float light_att_factor = 0.f;
 
	if (spot_light > cos(cut_off))
	{
		// Quadratic attenuation
		float linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit);
		light_att_factor = 1.f / max(pow(linear_att, 2), 1.f);
	}

	head_light = light_att_factor * (head_diffuse_light + head_specular_light);

	float d = distance(light_position, world_position);
	float attenuation_factor = 1.f / max(1.f + 0.0014f * d + 0.000007 * pow(d, 2), 1.f);
	light = head_light + ambient_light + attenuation_factor * (diffuse_light + specular_light);
	
	texcoord = v_texture_coord;
	
	//out_color = light *  texture2D(texture_1, texcoord);
	

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
