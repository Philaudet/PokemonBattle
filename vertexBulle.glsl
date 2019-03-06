#version 430 core

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 Normal;

out vec3 woldNormale;

uniform mat4 Model;
uniform mat4 Vue;
uniform mat4 Perspective;

void main()
{
	gl_Position = Perspective * Vue * Model * Position;
	woldNormale = (Model * vec4(Normal, 0.0)).xyz;
}