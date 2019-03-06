
#version 430 core

in vec2 TexCoords;
in vec3 worldNormale;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform float UniIntensite;

void main()
{
	float intensiteAmbiante = 0.1;
	if (UniIntensite > 0.0)
	{
		intensiteAmbiante = UniIntensite;
	}

	vec4 textureObjet = texture(texture_diffuse1, TexCoords);

	float diffuseFac = dot(normalize(worldNormale), -vec3(0.0f, -1.0f, 0.0f));
	vec4 DiffuseCol;

	if (diffuseFac > 0)
	{
		DiffuseCol = vec4((intensiteAmbiante * vec3(0.5f, 0.5f, 0.5f)) * vec3(0.5f, 0.5f, 0.5f) * diffuseFac, 1.0f);
	}
	else
	{
		DiffuseCol = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	color = textureObjet * (vec4(0.8f, 0.8f, 0.8f, 1.0f) + DiffuseCol);

	//color = texture(texture_diffuse1, TexCoords);
}


