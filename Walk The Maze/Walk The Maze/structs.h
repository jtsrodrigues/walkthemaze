#ifndef _MODELO_INCLUDE
#define _MODELO_INCLUDE


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef _WIN32
#include <GL/glaux.h>
#endif

#include "mathlib.h"
#include "studio.h"
#include "mdlviewer.h"

#pragma comment (lib, "glaux.lib")    /* link with Win32 GLAUX lib */
#pragma comment( user, "Compiled on " __DATE__ " at " __TIME__ ) 


extern "C" int read_JPEG_file(char *, char **, int *, int *, int *);

using namespace std;

#define NUM_TEXTURAS              2
#define MUNDOS	100
#define QUARTOS 10



// luzes e materiais

const GLfloat mat_ambient[][4] = { { 0.33, 0.22, 0.03, 1.0 },	// brass
{ 0.25, 0.0, 0.0 },			// red plastic
{ 0.0215, 0.1745, 0.0215 },	// emerald
{ 0.8, 0.8, 0.8 },		// slate
{ 0.0, 0.0, 0.1745 },		// azul
{ 0.02, 0.02, 0.02 },		// preto
{ 0.1745, 0.1745, 0.1745 } };// cinza

const GLfloat mat_diffuse[][4] = { { 0.78, 0.57, 0.11, 1.0 },		// brass
{ 0.5, 0.0, 0.0 },				// red plastic
{ 0.07568, 0.61424, 0.07568 },	// emerald
{ 0.78, 0.78, 0.78 },			// slate
{ 0.0, 0.0, 0.61424 },			// azul
{ 0.08, 0.08, 0.08 },			// preto
{ 0.61424, 0.61424, 0.61424 } };	// cinza

const GLfloat mat_specular[][4] = { { 0.99, 0.91, 0.81, 1.0 },			// brass
{ 0.2, 0.1, 0.1 },					// red plastic
{ 0.633, 0.727811, 0.633 },		// emerald
{ 0.14, 0.14, 0.14 },				// slate
{ 0.0, 0.0, 0.727811 },			// azul
{ 0.03, 0.03, 0.03 },				// preto
{ 0.727811, 0.727811, 0.727811 } };	// cinza

const GLfloat mat_shininess[] = { 27.8,	// brass
32.0,	// red plastic
76.8,	// emerald
18.78,	// slate
30.0,	// azul
75.0,	// preto
60.0 };	// cinza

enum tipo_material { brass, red_plastic, emerald, slate, azul, preto, cinza };

#ifdef __cplusplus
inline tipo_material operator++(tipo_material &rs, int) {
	return rs = (tipo_material)(rs + 1);
}
#endif

typedef	GLdouble Vertice[3];
typedef	GLdouble Vector[4];



typedef struct teclas_t{
	GLboolean   up, down, left, right, p,n,m;
}teclas_t;

//estutura de eixos 
typedef struct pos_t{
	GLfloat    x, y, z;
}pos_t;

typedef struct Donut{
	pos_t    pos;
	bool ativo;
	GLfloat dir;
}Donut;

#define MAX_POS 100000

// estutura dpara obejcto (usado no homer)
typedef struct objecto_t{
	pos_t    pos;
	GLfloat  dir;
	GLfloat  vel;
	GLfloat vida;
	Donut  donut[10];
	string posicoes;
}objecto_t;

typedef struct vista_t{
	boolean vistas[2]; //0 camera de cima 1 camera 3 pessoa

}vista_t;

typedef struct Camera{
	GLfloat fov;
	GLdouble dir_lat;
	GLdouble dir_long;
	GLfloat dist;
	Vertice center;

	pos_t    eye;


}Camera;



typedef struct Estado{
	Camera		camera;
	int			xMouse, yMouse;
	GLboolean	light;
	GLboolean	apresentaNormais;
	GLint		lightViewer;
	GLint		eixoTranslaccao;
	GLdouble	eixo[3];
	ALuint buffer, source, bufferChuva, sourceChuva, bufferGranizo, sourceGranizo, bufferNeve, sourceNeve, bufferBaterParede, sourceBaterParede, bufferDonut, sourceDonut, bufferExplosao, sourceExplosao;
	ALboolean tecla_s;
	ALboolean tecla_c;
	GLint		timer;
	GLint       delay;
	//teclas para o homer
	teclas_t  teclas;

	//Variaveis efeito dia/noite
	GLboolean noite;
	GLint tempoDia;

	//Variaveis particulas
	GLint tipoParticulas;
	GLint controloDeMudancaParticula;

	//rato press
	int mouseState;
}Estado;

typedef struct Modelo {
#ifdef __cplusplus
	tipo_material cor_cubo;
#else
	enum tipo_material cor_cubo;
#endif

	GLfloat g_pos_luz1[4];
	GLfloat g_pos_luz2[4];

	GLfloat escala;
	GLUquadric *quad;

	GLuint        texID[NUM_TEXTURAS];
	//variaveis para personagem 
	StudioModel   homer;   // Modelo da personagem
	GLboolean     andar;
	GLuint        prev;
	objecto_t	    objecto;
	GLint	vida;

}Modelo;
typedef struct ModeloInimigo {
#ifdef __cplusplus
	tipo_material cor_cubo;
#else
	enum tipo_material cor_cubo;
#endif

	GLfloat g_pos_luz1[4];
	GLfloat g_pos_luz2[4];

	GLfloat escala;
	GLUquadric *quad;

	GLuint        texID[NUM_TEXTURAS];
	//variaveis para personagem 
	StudioModel   inimigo;   // Modelo da personagem
	GLboolean     andar;
	GLuint        prev;
	objecto_t	    objecto;
	GLint	vida;
	bool visivel;

}ModeloInimigo;

//particulas

typedef struct {
	//Vida
	bool viva;
	float vida;
	float desaparecimento;
	//Posiçao
	float x;
	float y;
	float z;
	//Velocidade
	float vel;
	//Gravidade
	float grav;
}particulas;

//menu

typedef struct {

	GLboolean menuActivo;
	string mundoSelecionado;
	string mundos[MUNDOS];
	string fichMundos[MUNDOS];
	string quartoDeMundo[MUNDOS][QUARTOS];
	int totalMundos;

}Menu;

typedef struct {

	GLboolean loginActivo;
	string nomeUtilizador;
	string password;

}Login;

typedef struct {

	GLboolean FimMundoActivo;
	int pontuacao;
	GLboolean FimQuarto;

}FimMapa;

typedef struct {

	GLboolean FimQuartoActivo;
	int QuartoAnterior;
	int QuartoSeguinte;
	int i;
	int posinicio;
	int posfim;

}FimQuarto;

typedef struct FimMundo{
	StudioModel   modelo;
	pos_t pos;
	int mundoAnterior;
	int mundoSeguinte;
}FimMundo;

typedef struct Porta {
#ifdef __cplusplus
	tipo_material cor_cubo;
#else
	enum tipo_material cor_cubo;
#endif
	StudioModel   door;
	objecto_t	    objecto;
}Porta;

typedef struct Barril {
#ifdef __cplusplus
	tipo_material cor_cubo;
#else
	enum tipo_material cor_cubo;
#endif
	StudioModel   barel;
	objecto_t	    objecto;
}Barril;




#endif