
#include "Pokemon_Maker.h"

Pokemon_Maker::Pokemon_Maker()
{
}

Pokemon_Maker::Pokemon_Maker(glm::mat4 RotateInit, glm::vec3 Echelle, GLchar *PathPokemon)
{
	m_RotateInit = RotateInit;
	m_Echelle = Echelle;
	m_ModelPokemon = Model(PathPokemon);
	Core::Shader_Loader shaderLoader;
	m_ProgShader = shaderLoader.CreateProgram("ClassPokemon\\shader\\vertexPokemon.glsl", "ClassPokemon\\shader\\fragPokemon.glsl");
	glGetUniformLocation(m_ProgShader, "UniIntensite");
}

Pokemon_Maker::~Pokemon_Maker()
{
}

void Pokemon_Maker::Draw(glm::vec3 Position, glm::mat4 View, bool Hit)
{
	glm::mat4 Transformation;
	glm::mat4 projection;

	glUseProgram(m_ProgShader);

	if (Hit)
	{
		glUniform1f(LumAmbiante, 5.0f);
	}
	else
	{
		glUniform1f(LumAmbiante, 0.0f);
	}

	projection = glm::perspective(glm::radians(45.0f), (float)(glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT)), 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(m_ProgShader, "Vue"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_ProgShader, "Perspective"), 1, GL_FALSE, &projection[0][0]);

	Transformation = glm::translate(glm::mat4(1.0f), Position);
	Transformation = Transformation * m_RotateInit;
	Transformation = glm::scale(Transformation, m_Echelle);

	glUniformMatrix4fv(glGetUniformLocation(m_ProgShader, "Model"), 1, GL_FALSE, &Transformation[0][0]);

	m_ModelPokemon.Draw(m_ProgShader);
}

