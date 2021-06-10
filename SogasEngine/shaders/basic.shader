#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

out vec2 texCoord;

uniform float u_offset;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

void main()
{
	texCoord = a_texCoord;
	mat4 modelView = u_projection * u_view;
	gl_Position = modelView * u_model * vec4(a_position, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 texCoord;

uniform float u_color;
uniform sampler2D u_texture;

void main() 
{
	//color = vec4(u_color, 0.0, 1.0, 1.0);
	float flat_color = u_color;
	color = texture(u_texture, texCoord);
};