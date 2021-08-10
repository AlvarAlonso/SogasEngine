#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;

uniform mat4 u_viewprojection;
uniform mat4 u_model;

out vec3 v_uv;

void main()
{
	v_uv = a_position;
	v_uv.y *= -1;
	gl_Position = u_viewprojection * u_model * vec4(a_position, 1.0);
}

#shader fragment
#version 450 core

uniform samplerCube skybox;

in vec3 v_uv;

layout(location = 0) out vec4 FragColor;

void main()
{
	FragColor = texture(skybox, v_uv);
}