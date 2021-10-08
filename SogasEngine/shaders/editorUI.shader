#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;
layout(location = 3) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_viewprojectionMatrix;

out vec2 v_uv;

void main()
{
	v_uv = a_uv;
	gl_Position = u_viewprojectionMatrix * u_model * vec4(a_position, 1.0f);
}

#shader fragment
#version 450 core

uniform sampler2D u_texture;

in vec2 v_uv;

layout(location = 0) out vec4 outColor;

void main()
{
	if (texture(u_texture, v_uv).w < 0.5)
		discard;

	vec3 color = vec3(1);
	color *= texture(u_texture, v_uv).xyz;
	outColor = vec4(color, 1.0);
}