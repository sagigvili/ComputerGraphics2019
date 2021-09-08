#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// These outputs will be available in the fragment shader as inputs
out vec3 fragPos;
out vec3 fragNormal;

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	vec4 position = vec4(model * vec4(pos,1.0f));
	vec3 position1 = vec3(position) / position.w;
	fragPos = vec3(position) / position.w;
	fragNormal = vec3(model * vec4(normal,0.0f));

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0f);
	gl_Position = gl_Position / gl_Position.w;
}