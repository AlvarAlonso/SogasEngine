#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_worldPosition;

uniform float u_offset;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

void main()
{
	v_position = a_position;
	v_normal = (u_model * vec4(a_normal, 0.0)).xyz;
	v_worldPosition = (u_model * vec4(a_position, 1.0)).xyz;

	mat4 modelView = u_projection * u_view;
	gl_Position = modelView * (vec4(v_worldPosition, 1.0) + vec4(u_offset, 0, 0, 1));
};

#shader fragment
#version 330 core

in vec3 v_position;
in vec3 v_normal;
in vec3 v_worldPosition;

layout(location = 0) out vec4 color;

uniform float u_color;

vec3 lightPosition = vec3(0, 20, 0);

void main() 
{
	vec3 L = lightPosition - v_worldPosition;
	float NdotL = clamp(dot(normalize(v_normal), normalize(L)), 0.0, 1.0);

	color = vec4(1.0) * NdotL;
};