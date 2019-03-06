#version 430 core

in vec2 TexCoord;
in vec3 woldNormale;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{
	float intensiteAmbiante = 0.1;
	vec3 couleurLumAmbiante = vec3(1.0, 1.0, 1.0);
	vec3 DirectionLumiere = vec3(0.0, -1.0, 0.0);
	vec3 lumAmbiante = intensiteAmbiante * couleurLumAmbiante;

	float diffuseFac = dot(normalize(woldNormale), -DirectionLumiere);
	vec4 diffuseCol;
	if (diffuseFac > 0)
	{
		diffuseCol = vec4(couleurLumAmbiante * 0.8 * diffuseFac, 1.0);
	}
	else
	{
		diffuseCol = vec4(0.0, 0.0, 0.0, 1.0);
	}



	vec4 resultat = texture(texture_diffuse1, TexCoord) * (vec4(lumAmbiante,1.0) + diffuseCol);
	color = resultat;
} 