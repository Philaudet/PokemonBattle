/*********************************/
/*************   TP3   ***********/
/***   PAR Cédric Martineau    ***/
/*********************************/
#pragma once

#define SizeTabBulle 500
#define SizeTabRequin 1
#define WaitFrame 10
#define CombatResultMaxFrame 100
#define degreesToRadians(angleDegrees) (angleDegrees * 3.14159265359f / 180.0f)

#include <iostream>
#include "ShaderLoader\Shader_Loader.h"
#include "SkyboxLoader\Skybox_Loader.h"
#include "Dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "TextureLoader\Texture_Loader.h"
#include "ModelLoader\Model.h"
#include "ClassPokemon\Pokemon_Maker.h"
//#include "Dependencies\irrKlang-1.5.0\include\irrKlang.h"
#include <ctime>


//#pragma comment(lib, "irrKlang.lib") 


/***************************************************************************************/
// -----------------------------> Variables globales
/***************************************************************************************/

//irrklang::ISoundEngine* engine;

HWND g_HandleConsole;
GLuint skyboxProgram;
GLuint g_HandleTextSkybox;

Skybox::Skybox_Loader mySkybox;

glm::vec3 g_Position = glm::vec3(0.0f, 0.0f, -10.0f);
glm::vec3 g_Direction = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 g_Orientation = glm::vec3(0.0f, 1.0f, 0.0f);
float g_DernierX;
float g_DernierY;
bool g_Firsttime = true;
GLfloat g_Yaw = 90.0f;
GLfloat g_Pitch = 0.0f;

Model g_Stadium;
GLuint g_HandleLum;
GLuint g_ProgramStadium;
GLuint g_ProgramSA;

//*****************************
// ----> Combat et stats

int g_tour = 0;					// 0 : Tour du premier joueur, 1 : Tour du deuxieme joueur
Pokemon_Maker g_Player1;
Pokemon_Maker g_Player2;

int g_HPJoueur1 = 5;			// Hp des joueurs
int g_HPJoueur2 = 40;

int g_Phase = 1;				// 0 = Rest, 1 = Choix util, 2 = Normal Attack, 3 = Special Attack, 4 = Combat Result
int g_TurnFrame = 0;			// Nombre de frames écoulés
bool g_Avance = true;			// Si le pokemon attaque

bool GameStarted = false;
bool ModeAttack = false;
int g_AttackType = 0;

glm::vec3 g_PosiPlayer[1];
std::vector<char*> PathPoke;	// 0 : Snorlax, 1 : Charrizard 2: Venusaur
std::string g_PokemonChoisis[2];
//int Pokemon[2] = { -1, -1 };	// 0 : Snorlax, 1 : Charrizard

//*****************************
// ----> Fonctions
void LineWriter(std::string Text);
void InterfaceUtil(int Action, int Joueur, int DommageFait = 0);

void Deplacement();
void InitPosition();
void InitPathPoke();
void Wait(glm::mat4 *View);
void StartCombat(int value);
bool AnimNormalAttack(glm::mat4 *View);
void ChoixTypeAttack();
void AttackResult(glm::mat4 *View);

void DessinStadium(glm::mat4 Vue, bool Hit = false);

// initialisation des pokemons
void InitNewPokemon(int joueur, glm::mat4 rotate);
void InitCharrisard(int joueur, glm::mat4 rotate);
void InitSnorlax(int joueur, glm::mat4 rotate);
void InitBlastoise(int joueur, glm::mat4 rotate);


/***************************************************************************************/
// -----------------------------> Rendu de la scène
/***************************************************************************************/
void renduScene()
{
	//variables locales
	glm::mat4 Transformation;
	glm::mat4 view;
	glm::mat4 projection;

	Deplacement();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Création des matrices de projection et de vue
	projection = glm::perspective(glm::radians(45.0f), (float)(glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT)), 0.1f, 100.0f);
	view = glm::lookAt(g_Position, g_Direction + g_Position, g_Orientation);

	//**********************************
	// ----> Controlleur du combat
	bool Hit = false;

	if (GameStarted)
	{
		switch (g_Phase)
		{
		case 0:
			Wait(&view);
			break;
		case 1:
			ChoixTypeAttack();
			break;
		case 2:
			Hit = AnimNormalAttack(&view);
			break;
		case 3:
			Hit = AnimNormalAttack(&view);
			break;
		case 4:
			AttackResult(&view);
			break;
		default:
			break;
		}
	}

	//**********************************
	// ----> Dessin des objets

	//Dessin du skybox avec la fonction drawSkybox du Skybox_Loader
	//glm::mat4 viewSkybox = glm::mat4(glm::mat3(view));
	//mySkybox.drawSkybox(viewSkybox, projection, Hit);

	DessinStadium(view,Hit);

	g_Player1.Draw(g_PosiPlayer[0],view, Hit);
	g_Player2.Draw(g_PosiPlayer[1], view, Hit);

	//glutSetVertexAttribCoord3(0);
	//glutSetVertexAttribNormal(1);
	//glutSolidSphere(0.5, 30, 30);

	glutSwapBuffers();

}

/***************************************************************************************/
// -----------------------------> Dessin des objets
/***************************************************************************************/
void DessinStadium(glm::mat4 Vue, bool Hit)
{
	GLint UniModel, UniVue, UniPerspective;

	glUseProgram(g_ProgramStadium);

	UniModel = glGetUniformLocation(g_ProgramStadium, "gModele");
	UniVue = glGetUniformLocation(g_ProgramStadium, "gVue");
	UniPerspective = glGetUniformLocation(g_ProgramStadium, "gProjection");

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 7.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Mat de model
	glm::mat4 Model = glm::mat4(1.0f);
	Model = glm::translate(glm::mat4(), glm::vec3(0.0f, -10.0f, 95.0f));
	Model = glm::rotate(Model, degreesToRadians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	Model = glm::rotate(Model, degreesToRadians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	Model = glm::scale(Model, glm::vec3(15.0f, 15.0f, 15.0f));
	glUniformMatrix4fv(UniModel, 1, GL_FALSE, &Model[0][0]);

	// Mat de vue

	glUniformMatrix4fv(UniVue, 1, GL_FALSE, &Vue[0][0]);

	if (Hit)
	{
		glUniform1f(g_HandleLum, 10.0f);
	}
	else
	{
		glUniform1f(g_HandleLum, 0.0f);
	}

	// Mat de Perspective
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 500.0f);
	glUniformMatrix4fv(UniPerspective, 1, GL_FALSE, &Projection[0][0]);

	g_Stadium.Draw(g_ProgramStadium);
}


/***************************************************************************************/
// -----------------------------> Interface
/***************************************************************************************/

void LineWriter(std::string Text)
{
	int LongueurLigne = 57 - Text.length();

	std::string StringToWrite = "::";
	for (int i = 0; i < LongueurLigne / 2; i++)
	{
		StringToWrite += " ";
	}

	StringToWrite += Text;

	for (int i = 0; i < LongueurLigne / 2 + LongueurLigne % 2; i++)
	{
		StringToWrite += " ";
	}
	StringToWrite += "::\n";
	std::cout << StringToWrite;

}

// Action -> 0: Début, 1: Action Combat, 2: Dommage Combat, 3: Resultat Combat, 4: Joueur Gagnant
void InterfaceUtil(int Action, int Joueur, int DommageFait)
{
	int NouvellePartie = -1;			// A changer

	system("cls");
	std::string Text;

	switch (Action)
	{
	case 0:
		std::cout << "::<-0->=====================<-O->=====================<-0->::\n";
		LineWriter("");
		LineWriter("Bienvenue dans Pokemon Arena");
		LineWriter("");
		std::cout << "::<-0->=====================<-O->=====================<-0->::\n";
		break;
	case 1:
		std::cout << "::<-0->=====================<-O->=====================<-0->::\n";
		LineWriter("");
		//HP Joueur 1
		LineWriter("Joueur 1 : " + std::to_string(g_HPJoueur1));
		//HP Joueur 2
		LineWriter("Joueur 2 : " + std::to_string(g_HPJoueur2));
		LineWriter("");

		std::cout << "::<-0->=====================<-O->=====================<-0->::\n";
		break;
	case 3:
		std::cout << "::<-0->=====================<-O->=====================<-0->::\n";
		LineWriter("");
		LineWriter(g_PokemonChoisis[Joueur] + " a fait " + std::to_string(DommageFait) + " de dommage");
		LineWriter("");
		std::cout << "::<-0->=====================<-O->=====================<-0->::\n";
		break;
	case 4:
		std::cout << "::<-0->=====================<-O->=====================<-0->::\n";
		LineWriter("");
		LineWriter("");
		LineWriter(g_PokemonChoisis[Joueur] + " a gagne la partie !!");
		LineWriter("");
		std::cout << "::<-0->=====================<-O->=====================<-0->::\n";
		LineWriter("");
		LineWriter("");
		LineWriter("Voulez-vous faire une nouvelle partie ?");
		LineWriter("");
		LineWriter("");
		std::cout << ":: ---> ";

		std::cin >> NouvellePartie;
		break;

	default:
		break;
	}
}

void ChoixTypeAttack()
{
	//InitPathPoke();

	bool PremierreRequete = true;

	SetWindowPos(g_HandleConsole, HWND_TOPMOST, 0, 0, 530, 325, SWP_SHOWWINDOW);

	g_AttackType = -1;
	while (g_AttackType < 1 || g_AttackType > 2)
	{
		InterfaceUtil(1, 0);
		if (!PremierreRequete)
		{
			LineWriter("La valeur entrée n'est pas valide !");
			LineWriter("");
		}
		LineWriter("");
		LineWriter("Quelle attaque voulez-vous faire ?");
		LineWriter("");

		LineWriter("1 : Tackle");
		LineWriter("2 : Special");
		LineWriter("");
		std::cout << "::<-0->=====================<-O->=====================<-0->::\n";
		LineWriter("");
		std::cout << ":: ---> ";

		std::cin >> g_AttackType;

		PremierreRequete = false;
	}

	switch (g_AttackType)
	{
	case 1:
		g_Phase = 2;
		break;
	case 2:
		g_Phase = 3;
		break;
	default:
		break;
	}

	ModeAttack = true;

	SetWindowPos(g_HandleConsole, HWND_TOPMOST, 0, 0, 530, 325, SWP_SHOWWINDOW);
}

void AttackResult(glm::mat4 *View)
{
	srand((int)time(0));
	int DamageDone = 0;
	int JoueurAttaquant = (g_tour - 1) % 2;

	//**********************************************
	// ----> Calcul du dommage
	// Tackle
	if (g_AttackType == 1)
	{
		DamageDone = 3 + (rand() % 4);
	}
	// Special
	else
	{
		DamageDone = 5 + (rand() % 4);
	}

	//**********************************************
	// ----> Affichage du dommage

	// Premier joueur viens d'attaquer
	if (g_TurnFrame == 0)
	{
		if (JoueurAttaquant == 0)
		{
			InterfaceUtil(3, 0, DamageDone);

			g_HPJoueur2 -= DamageDone;

			if (g_HPJoueur2 < 0)
			{
				InterfaceUtil(4, 1);
			}
		}
		// Si le deuxieme joueur viens d'attaquer
		else
		{
			InterfaceUtil(3, 1, DamageDone);

			g_HPJoueur1 -= DamageDone;

			if (g_HPJoueur1 < 0)
			{
				InterfaceUtil(4, 0);
			}
		}
	}
	
	g_TurnFrame++;

	*View = glm::lookAt(g_Position, g_PosiPlayer[JoueurAttaquant], g_Orientation);

	if (g_TurnFrame >= CombatResultMaxFrame)
	{
		g_Phase = 1;
		g_TurnFrame = 0;
		*View = glm::lookAt(g_Position, g_PosiPlayer[(JoueurAttaquant + 1) % 2], g_Orientation);
	}
}

/***************************************************************************************/
// -----------------------------> Animations
/***************************************************************************************/
bool AnimNormalAttack(glm::mat4 *View)
{
	bool HitFlash = false;

	//**********************************************
	// ----> Si c'est le tour du premier joueur
	if (g_tour % 2 == 0)
	{
		// Faire avancer le pokemon
		if (g_Avance)
		{
			g_PosiPlayer[0].x += 0.5f;

			if (g_PosiPlayer[0].x >= 0.0f)
			{
				g_Avance = false;
			}
		}
		// Faire reculer le pokemon
		else
		{
			g_PosiPlayer[0].x -= 0.5f;

			if (g_PosiPlayer[0].x >= -3.0)
			{
				HitFlash = true;
			}

			if (g_PosiPlayer[0].x <= -10.0f)
			{
				g_Avance = true;
				ModeAttack = false;
				g_tour++;
				g_Phase = 4;
				g_TurnFrame = 0;
			}
		}

		*View = glm::lookAt(g_Position, g_PosiPlayer[0], g_Orientation);

	}
	//**********************************************
	// ----> Si c'est le tour du deuxième joueur
	else
	{
		// Faire avancer le pokemon
		if (g_Avance)
		{
			g_PosiPlayer[1].x -= 0.5f;

			if (g_PosiPlayer[1].x <= 0.0f)
			{
				g_Avance = false;
			}
		}
		// Faire reculer le pokemon
		else
		{
			g_PosiPlayer[1].x += 0.5f;

			if (g_PosiPlayer[1].x <= 3.0)
			{
				HitFlash = true;
			}

			if (g_PosiPlayer[1].x >= 10.0f)
			{
				g_Avance = true;
				ModeAttack = false;
				g_tour++;
				g_Phase = 4;
				g_TurnFrame = 0;
			}
		}
		*View = glm::lookAt(g_Position, g_PosiPlayer[1], g_Orientation);
	}

	return HitFlash;
}

bool AnimSpecialAttack(glm::mat4 *View)
{
	if (g_tour % 2 == 0)
	{

	}
	else
	{

	}

	return true;
}

void StartCombat(int value)
{
	//engine = irrklang::createIrrKlangDevice();
	//
	//if (!engine)
	//	return; // error starting up the engine
	//			//engine->play2D("Song/PokemonMasterBattleDiamond.mp3", true);

	g_tour = (rand() % 2) - 1;

	GameStarted = true;

	glutPostRedisplay();
}

void Wait(glm::mat4 *View)
{
	*View = glm::lookAt(g_Position, g_PosiPlayer[g_tour % 2], g_Orientation);
	if (g_TurnFrame < WaitFrame)
	{
		g_TurnFrame++;
	}

	if (g_TurnFrame >= WaitFrame)
	{
		g_TurnFrame = 0;
		g_Phase = 1;
	}
	glutPostRedisplay();
}


void InitPathPoke()
{
	PathPoke.push_back("Pokemon3D/Snorlax/snorlax.FBX");
	PathPoke.push_back("Pokemon3D/Charizard/Charizard2.FBX");
	PathPoke.push_back("Pokemon3D/Blastoise/Blastoise2.FBX");
}

void InitPosition()
{
	//Set Position Player
	g_PosiPlayer[0] = glm::vec3(-10.0f, -1.0f, 2.5f);
	g_PosiPlayer[1] = glm::vec3(10.0f, -1.0f, 2.5f);
}

void Deplacement()
{
	GLfloat Vitesse = 0.2f;

	glm::vec3 Front;

	Front.x = cosf(glm::radians(g_Yaw)) * cosf(glm::radians(g_Pitch));
	Front.y = sinf(glm::radians(g_Pitch));
	Front.z = sinf(glm::radians(g_Yaw)) * cosf(glm::radians(g_Pitch));

	g_Direction = glm::normalize(Front);
}

/***************Fonction appelée pour la gestion de la fermeture de l'application******************/
void fermeture()
{
	glutLeaveMainLoop();
}

/***************Fonction appelée pour la gestion du mouvement de la souris******************/
void callbackmotion(int x, int y)
{
	if (g_Firsttime == true)
	{
		g_Firsttime = false;
		g_DernierX = x;
		g_DernierY = y;
	}
	else
	{
		GLfloat decalageX = x - g_DernierX;
		g_DernierX = x;

		decalageX *= 0.05f;
		g_Yaw += decalageX;

		GLfloat decalageY = y - g_DernierY;
		g_DernierY = y;

		decalageY *= -0.05f;
		g_Pitch += decalageY;

		if (g_Pitch > 89)
		{
			g_Pitch = 89;
		}
		if (g_Pitch < -89)
		{
			g_Pitch = -89;
		}

		g_Firsttime = true;
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	}
	glutPostRedisplay();
}

bool ChoixInitialUtilisateur()
{
	SetWindowPos(g_HandleConsole, HWND_TOPMOST, 0, 0, 530, 300, SWP_SHOWWINDOW);

	InitPosition();
	InitPathPoke();

	//**************************************
	// ----> Choix du premier utilisateur

	InterfaceUtil(0, 0);
	LineWriter("");
	LineWriter("Quelle Pokemon desirez le premier joueur?");
	LineWriter("0 : Snorlax");
	LineWriter("1 : Charizard");
	LineWriter("2 : Blastoise");
	LineWriter("");
	std::cout << ":: ------> ";

	int IndicePoke1;
	glm::mat4 rotate = glm::mat4(1.0f);

	std::cin >> IndicePoke1;

	if (IndicePoke1 < 0 || IndicePoke1 >= PathPoke.size())
	{
		return false;
	}

	switch (IndicePoke1)
	{
	case 0:
		InitSnorlax(1, rotate);
		break;
	case 1:
		InitCharrisard(1, rotate);
		break;
	case 2:
		InitBlastoise(1, rotate);
		break;
	default:
		InitSnorlax(1, rotate);
		break;
	}

	//**************************************
	// ----> Choix du deuxieme utilisateur

	LineWriter("");
	LineWriter("Quelle Pokemon desirez le deuxieme joueur?");
	LineWriter("");
	std::cout << ":: ------> ";

	int IndicePoke2;

	std::cin >> IndicePoke2;

	if (IndicePoke2 < 0 || IndicePoke2 >= PathPoke.size())
	{
		return false;
	}

	rotate = glm::mat4(1.0f);
	rotate = glm::rotate(rotate, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	switch (IndicePoke2)
	{
	case 0:
		InitSnorlax(2, rotate);
		break;
	case 1:
		InitCharrisard(2, rotate);
		break;
	case 2:
		InitBlastoise(2, rotate);
		break;

	default:
		InitSnorlax(2, rotate);
		break;
	}

	//**************************************
	// ----> Initialisation du reste

	SetWindowPos(g_HandleConsole, HWND_TOPMOST, 0, 0, 530, 300, SWP_SHOWWINDOW);

	// Initialisation du stadium
	g_Stadium = Model("Super Training Stadium/SuperTrainingStadium.FBX");

	return true;
}

void InitNewPokemon(int joueur, glm::mat4 rotate)
{
	g_PokemonChoisis[joueur - 1] = "Nouv. Pokemon";
	rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Joueur 1
	if (joueur == 1)
	{
		g_Player1 = Pokemon_Maker(rotate, glm::vec3(0.01f, 0.01f, 0.01f), PathPoke[3]);
	}
	// Joueur 2
	else
	{
		g_Player2 = Pokemon_Maker(rotate, glm::vec3(0.01f, 0.01f, 0.01f), PathPoke[3]);
	}
}

// Joueur : 1 ou 2 
void InitBlastoise(int joueur, glm::mat4 rotate)
{
	g_PokemonChoisis[joueur - 1] = "Blastoise";

	rotate = glm::rotate(rotate, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	rotate = glm::rotate(rotate, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	
	// Joueur 1
	if (joueur == 1)
	{
		g_Player1 = Pokemon_Maker(rotate, glm::vec3(0.01f, 0.01f, 0.01f), PathPoke[2]);
	}
	// Joueur 2
	else
	{
		g_Player2 = Pokemon_Maker(rotate, glm::vec3(0.01f, 0.01f, 0.01f), PathPoke[2]);
	}
}

// Joueur : 1 ou 2 
void InitCharrisard(int joueur, glm::mat4 rotate)
{
	g_PokemonChoisis[joueur - 1] = "Charrisard";

	rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Joueur 1
	if (joueur == 1)
	{
		g_Player1 = Pokemon_Maker(rotate, glm::vec3(0.01f, 0.01f, 0.01f), PathPoke[1]);
	}
	// Joueur 2
	else
	{
		g_Player2 = Pokemon_Maker(rotate, glm::vec3(0.01f, 0.01f, 0.01f), PathPoke[1]);
	}
}

// Joueur : 1 ou 2 
void InitSnorlax(int joueur, glm::mat4 rotate)
{
	g_PokemonChoisis[joueur - 1] = "Snorlax";

	rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Joueur 1
	if (joueur == 1)
	{
		g_Player1 = Pokemon_Maker(rotate, glm::vec3(0.01f, 0.01f, 0.01f), PathPoke[0]);
	}
	// Joueur 2
	else
	{
		g_Player2 = Pokemon_Maker(rotate, glm::vec3(0.01f, 0.01f, 0.01f), PathPoke[0]);
	}
}

/***************Fonction appelée pour la gestion des touches du clavier***************************/
void clavier(unsigned char touche, int x, int y)
{
	if (touche == 27)
	{
		fermeture();
	}

}

/********************Fonction de création du skybox (cube et cubemap) ******************************************/
bool initSkybox()
{

	std::vector<char*> faces;
	faces.push_back("skybox/desertsky_rt.tga");
	faces.push_back("skybox/desertsky_lf.tga");
	faces.push_back("skybox/desertsky_up.tga");
	faces.push_back("skybox/desertsky_dn.tga");
	faces.push_back("skybox/desertsky_bk.tga");
	faces.push_back("skybox/desertsky_ft.tga");

	mySkybox.creeSkybox(&skyboxProgram, faces);


	return true;

}

/**************************Fonction de création des programmes de shaders GLSL *******************/
void initShaderProgram()
{
	Core::Shader_Loader shaderLoader;
	skyboxProgram = shaderLoader.CreateProgram("skyboxVS.glsl", "skyboxFS.glsl");
	g_ProgramStadium = shaderLoader.CreateProgram("VertexStadium.glsl", "FragStadium.glsl");
	g_ProgramSA = shaderLoader.CreateProgram("VertexSA.glsl", "FragSA.glsl");

	g_HandleLum = glGetUniformLocation(g_ProgramStadium, "UniIntensite");

	InitPosition();
	
}

/****************Fonction de paramétrage d'OpenGL************************/
/****************appelée par la fonction main ***************************/
void parametrerGL()
{
	//Mode d'affichage des polygones dessinés
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Cacher le curseur de souris
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);

	//Activer la profondeur pour le rendu 3D
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Activer le camouflage des coins du skybox
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

}

void Refresh()
{
	glutPostRedisplay();
}

/**********************Fonction principale d'initialisation et d'exécution*************************/
int main(int argc, char **argv)
{
	g_HandleConsole = GetConsoleWindow();
	//Configuration et création de la fenêtre freeGLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Pokemon");
	glutFullScreen();
	glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
	
	//Initialisation de GLEW
	glewInit();

	//Appel de la fonction de paramétrage d'OpenGL
	parametrerGL();

	/********Fonctions de rappel**********/

	glutDisplayFunc(renduScene);
	glutCloseFunc(fermeture);
	glutKeyboardFunc(clavier);
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	glutIdleFunc(Refresh);
	//glutPassiveMotionFunc(callbackmotion);

	//Création des programmes de shaders
	initShaderProgram();

	srand((int)time(0));

	//Initialisation du skybox
	if (!initSkybox())
	{
		return -1;
	}

	ChoixInitialUtilisateur();
	
	glutTimerFunc(250, StartCombat, 0);

	//Boucle principale d'exécution
	glutMainLoop();

	//engine->drop();

	return 0;
}
