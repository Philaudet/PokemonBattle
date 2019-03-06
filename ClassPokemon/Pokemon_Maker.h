#pragma once
#include "..\ShaderLoader\Shader_Loader.h"
#include "..\Dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "..\TextureLoader\Texture_Loader.h"
#include "..\ModelLoader\Model.h"

class Pokemon_Maker
{
public:
	Pokemon_Maker();
	Pokemon_Maker(glm::mat4 RotateInit, glm::vec3 Echelle, GLchar *PathPokemon);
	~Pokemon_Maker();

	int GetHeal()
	{
		return Heal;
	}

	void Draw(glm::vec3 Position, glm::mat4 View,bool Hit = false);
private:
	glm::vec3 m_Position;
	glm::vec3 m_Echelle;
	glm::mat4 m_RotateInit;
	Model m_ModelPokemon;
	GLuint m_ProgShader;
	GLuint LumAmbiante;

	int Heal = 100;
};
