#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform float offset;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 modelView = projection * view;
	gl_Position = modelView * vec4(position + vec4(offset, 0, 0, 1));
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_color;

void main() 
{
	color = vec4(u_color, 0.0, 1.0, 1.0);
};