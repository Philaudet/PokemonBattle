#version 430 core
in vec3 TexCoords;


out vec4 color;

uniform samplerCube skybox;

uniform float UniIntensite;

void main()
{    
	float intensiteAmbiante = 0.5f;
	if (UniIntensite > 0.0)
	{
		intensiteAmbiante = UniIntensite;
	}
	vec3 couleurLumAmbiante = vec3(1.0, 1.0, 1.0);
	vec3 lumAmbiante = intensiteAmbiante * couleurLumAmbiante;


	vec4 resultat = vec4(lumAmbiante, 1.0f) * texture(skybox, TexCoords);
    color = resultat;
}