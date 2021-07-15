#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;
layout(location = 3) in vec2 a_uv;
//layout(location = 4) in int a_entityID;

uniform float u_offset;
uniform int u_entityID;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_worldPosition;
out vec2 v_uv;
out vec3 v_color;
out flat int v_entityID;

void main()
{
	v_position = a_position;
	v_normal = (u_model * vec4(a_normal, 0.0)).xyz;
	v_worldPosition = (u_model * vec4(a_position, 1.0)).xyz;
	v_uv = a_uv;
	v_color = a_color;

	v_entityID = u_entityID;

	mat4 modelView = u_projection * u_view;
	gl_Position = modelView * u_model * vec4(a_position, 1.0);
};

#shader fragment
#version 450 core

in vec3 v_position;
in vec3 v_normal;
in vec3 v_worldPosition;
in vec2 v_uv;
in vec3 v_color;
in flat int v_entityID;

layout(location = 0) out vec4 outColor;
layout(location = 1) out int outColor2;

uniform sampler2D u_texture;

uniform vec3 lightPosition;
uniform vec3 lightColor;

void main() 
{
	vec3 L = normalize(lightPosition - v_worldPosition);
	vec3 N = normalize(v_normal);
	float NdotL = clamp(dot(N, L), 0.0, 1.0);
	vec2 uv = v_uv.xy;
	vec3 color = v_color * lightColor;
	color *= texture(u_texture, uv).xyz;
	outColor = NdotL * vec4(color, 1);
	outColor2 = v_entityID;
};