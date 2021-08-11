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
	vec4 position = u_viewprojection * u_model * vec4(a_position, 1.0);

	/*
	* To reduce the number of fragment computations, we will render the skybox last instead of first. This way we will
	* shader only the visible fragments in the skybox.
	* To do so we have to trick the pipeline by telling the depth is equal to 1.0 (the maximum depth in openGL), thus we
	* will pass z = w. This is because after vertex pass a perspective division will be performed on gl_Position, dividing
	* the components by its w value. The z component will then be the depth value.
	*/

	gl_Position = position.xyww;
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