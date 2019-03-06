#include "Skybox_Loader.h"
#include "..\TextureLoader\Texture_Loader.h"
#include<iostream>
#include<vector>

using namespace Skybox;
using namespace texCore;


Skybox::Skybox_Loader::Skybox_Loader(void)
{
}

Skybox::Skybox_Loader::~Skybox_Loader(void)
{
}

/****************Fonction permettant la cr�ation du skybox ********************/
void Skybox::Skybox_Loader::creeSkybox(GLuint* program, std::vector<char*> faces)
{
	Texture_Loader textureload;
	skyboxProgram = *program;

	glUseProgram(skyboxProgram);

	glGetUniformLocation(skyboxProgram, "UniIntensite");
	//Cr�ation du cube dans un VAO
	glGenVertexArrays(1, &vaoCube);
	glBindVertexArray(vaoCube);
	drawCube(1.0f, 1.0f, 1.0f);
	glBindVertexArray(0);

	//Cr�ation de la texture Cubemap
	skyboxTexture = textureload.loadCubemap(faces);
}

/******************Fonction permettant le dessin du skybox *********************/
void Skybox::Skybox_Loader::drawSkybox(glm::mat4 vue, glm::mat4 proj, bool Hit)
{
	GLuint hprojection, hvue;

	glUseProgram(skyboxProgram);

	//Variables uniform du programme
	hprojection = glGetUniformLocation(skyboxProgram, "gProjection");
	assert(hprojection != 0xFFFFFFFF);
	hvue = glGetUniformLocation(skyboxProgram, "gVue");
	assert(hvue != 0xFFFFFFFF);
	
	glUniformMatrix4fv(hprojection, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(hvue, 1, GL_FALSE, &vue[0][0]);

	if (Hit)
	{
		glUniform1f(LumAmbiante, 5.0f);
	}
	else
	{
		glUniform1f(LumAmbiante, 0.0f);
	}

	// Appliquer la texture
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(skyboxProgram, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	//Dessiner le cube
	glBindVertexArray(vaoCube);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	
}

/*******************Fonction priv�e appel�e par creeSkybox() pour la cr�ation du cube du skybox ************/
void Skybox::Skybox_Loader::drawCube(float largeur, float hauteur, float profondeur)
{
	GLfloat skyboxVertices[] = {
		// Positions          
		-largeur,  hauteur, -profondeur,// arri�re
		-largeur, -hauteur, -profondeur,
		largeur, -hauteur, -profondeur,
		largeur, -hauteur, -profondeur,
		largeur,  hauteur, -profondeur,
		-largeur,  hauteur, -profondeur,

		-largeur, -hauteur,  profondeur, //c�t� gauche
		-largeur, -hauteur, -profondeur,
		-largeur,  hauteur, -profondeur,
		-largeur,  hauteur, -profondeur,
		-largeur,  hauteur,  profondeur,
		-largeur, -hauteur,  profondeur,

		largeur, -hauteur, -profondeur, //c�t� droit
		largeur, -hauteur,  profondeur,
		largeur,  hauteur,  profondeur,
		largeur,  hauteur,  profondeur,
		largeur,  hauteur, -profondeur,
		largeur, -hauteur, -profondeur,

		-largeur, -hauteur,  profondeur, //avant
		-largeur,  hauteur,  profondeur,
		largeur,  hauteur,  profondeur,
		largeur,  hauteur,  profondeur,
		largeur, -hauteur,  profondeur,
		-largeur, -hauteur,  profondeur,

	
		largeur,  hauteur, -profondeur,	 //dessus
		largeur,  hauteur,  profondeur,
		-largeur,  hauteur,  profondeur,
		-largeur,  hauteur,  profondeur,
		largeur,  hauteur, -profondeur,
		-largeur,  hauteur, -profondeur,
		

		-largeur, -hauteur, -profondeur, //dessous
		-largeur, -hauteur,  profondeur,
		largeur, -hauteur, -profondeur,
		largeur, -hauteur, -profondeur,
		-largeur, -hauteur,  profondeur,
		largeur, -hauteur,  profondeur
	};

	GLuint skyboxVBO;
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}
