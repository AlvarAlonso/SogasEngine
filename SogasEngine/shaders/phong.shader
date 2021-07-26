#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;
layout(location = 3) in vec2 a_uv;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_model;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_worldPosition;
out vec2 v_uv;
out vec3 v_color;

void main()
{
	mat4 model = u_model;
	v_position = a_position;
	v_normal = (model * vec4(a_normal, 0.0)).xyz;
	v_worldPosition = (model * vec4(a_position, 1.0)).xyz;
	v_uv = a_uv;
	v_color = a_color;

	gl_Position = u_viewProjectionMatrix * model * vec4(a_position, 1.0);
};

#shader fragment
#version 330 core

in vec3 v_position;
in vec3 v_normal;
in vec3 v_worldPosition;
in vec2 v_uv;
in vec3 v_color;

layout(location = 0) out vec4 outColor;

uniform sampler2D u_texture;

uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;

void main()
{
	float maxDistance = 75;

	vec3 L = v_worldPosition - u_lightPosition;
	float lightDistance = length(L);
	L = normalize(L);

	float attenuation = maxDistance - lightDistance;
	attenuation /= maxDistance;
	attenuation = max(attenuation, 0.0);
	attenuation = attenuation * attenuation;

	vec3 N = normalize(v_normal);
	float NdotL = clamp(dot(N, L), 0.0, 1.0);
	vec2 uv = v_uv;
	vec3 color = v_color * u_lightColor;
	color *= texture(u_texture, uv).xyz;
	outColor = NdotL * vec4(color, 1) * attenuation;
	//outColor = vec4(1, 0, 0, 1);
};