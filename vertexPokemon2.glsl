#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 texCoord;

out vec2 TexCoord;
out vec3 woldNormale;

uniform mat4 Model;
uniform mat4 Vue;
uniform mat4 Perspective;

void main()
{
	vec4 PosiModif = vec4(Position.x, Position.y, Position.z, 1.0);
	gl_Position = Perspective * Vue * Model * PosiModif;
	TexCoord = vec2(texCoord.x, texCoord.y);
	woldNormale = (Model * vec4(Normal, 0.0)).xyz;
}