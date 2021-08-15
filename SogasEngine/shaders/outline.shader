#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;
layout(location = 3) in vec2 a_uv;

uniform mat4 u_viewprojection;
uniform mat4 u_model;

void main()
{
	mat4 model = u_model;
	float offset = 0.05f;

	gl_Position = u_viewprojection * u_model * vec4(a_position, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = vec4(1, 0.65, 0, 1);
}