#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

uniform mat4 u_model;
uniform mat4 u_viewprojectionMatrix;
uniform vec3 u_color;

out vec4 v_color;

void main()
{
	v_color = vec4(u_color, 1.0);
	gl_Position = u_viewprojectionMatrix * u_model * vec4(a_position, 1.0);
}

#shader fragment
#version 450 core

in vec4 v_color;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = v_color;
}