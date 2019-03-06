#version 430 core

in vec2 TexCoord;
in vec3 woldNormale;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform float UniIntensite;

void main()
{
	float intensiteAmbiante = 0.5f;
	if (UniIntensite > 0.0)
	{
		intensiteAmbiante = UniIntensite;
	}
	vec3 couleurLumAmbiante = vec3(1.0, 1.0, 1.0);
	vec3 DirectionLumiere = vec3(0.0, -1.0, 0.0);
	vec3 lumAmbiante = intensiteAmbiante * couleurLumAmbiante;

	float diffuseFac = dot(normalize(woldNormale), -DirectionLumiere);
	vec4 diffuseCol;
	if (diffuseFac > 0)
	{
		diffuseCol = vec4(lumAmbiante * intensiteAmbiante * diffuseFac, 1.0);
	}
	else
	{
		diffuseCol = vec4(0.0, 0.0, 0.0, 1.0);
	}



	vec4 resultat = texture(texture_diffuse1, TexCoord) * (vec4(lumAmbiante,1.0) + diffuseCol);
	color = resultat;
} 