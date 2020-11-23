#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

void main()
{
	// TODO: compute world space vectors
	vec3 world_normal = normalize((Model * vec4(v_normal, 0.0)).xyz);
	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;

	vec3 L = normalize(light_position - world_pos); //vectorul directiei luminii
	//vectorul directiei in care priveste observatorul
	vec3 V = normalize(eye_position - world_pos);

	vec3 N = normalize(world_normal);
	vec3 H = normalize(L + V); //vectorul median

	// TODO: define ambient light component
	float ambient_light = 0.25;

	// TODO: compute diffuse light component
	//float diffuse_light = material_kd * max (dot(N, L), 0);
	float diffuse_light = dot(L, world_normal);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		//directia R este simetrica cu L fata de normala la suprafata
		vec3 R = reflect(-L, world_normal);
		specular_light = max(pow(dot(V, R), material_shininess), 0);
	}

	// TODO: compute light
	float light = diffuse_light + ambient_light + specular_light;
	
	// TODO: send color light output to fragment shader
	//color = vec3(diffuse_light, diffuse_light, diffuse_light);
	//color = vec3(specular_light, specular_light, specular_light);
	//color = object_color * light;
	color = (max(diffuse_light, ambient_light) * material_kd + specular_light * material_ks) * object_color;
	
	

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
