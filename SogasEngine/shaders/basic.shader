#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

uniform float u_offset;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

void main()
{
	mat4 modelView = u_projection * u_view;
	gl_Position = modelView * u_model * vec4(a_position, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_color;

void main() 
{
	color = vec4(u_color, 0.0, 1.0, 1.0);
};