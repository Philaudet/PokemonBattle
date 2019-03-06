
#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 gModele;
uniform mat4 gVue;
uniform mat4 gProjection;

out vec2 TexCoords;
out vec3 worldNormale;

void main()
{
	gl_Position = gProjection * gVue * gModele * vec4(position, 1.0f);
	TexCoords = texCoords;
	worldNormale = (gModele * vec4(normal, 0.0)).xyz;
}


