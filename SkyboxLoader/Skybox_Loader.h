#pragma once

#include "..\Dependencies\glew\glew.h"
#include "..\Dependencies\freeglut\freeglut.h"
#include "..\Dependencies\glm\glm\glm.hpp"
#include <iostream>
#include <vector>

namespace Skybox
{
	class Skybox_Loader
	{
	public:
		Skybox_Loader(void);
		~Skybox_Loader(void);
				
		void creeSkybox(GLuint* program, std::vector<char*> faces);
		void drawSkybox(glm::mat4 vue, glm::mat4 proj, bool Hit = false);
		
	private:
		GLuint skyboxTexture;
		GLuint skyboxProgram;
		GLuint LumAmbiante;
		GLuint vaoCube, vboSommets;

		void drawCube(float largeur, float hauteur, float profondeur);

	
	
	};
}


