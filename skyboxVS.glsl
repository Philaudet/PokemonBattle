#version 430 core
layout (location = 0) in vec3 position;

out vec3 TexCoords;


uniform mat4 gProjection;
uniform mat4 gVue;

void main()
{
    vec4 pos = gProjection * gVue * vec4(position, 1.0);
    gl_Position = vec4(pos.xy,1.0,1.0);
    TexCoords = position;

} 
