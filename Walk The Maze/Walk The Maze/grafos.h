#ifndef _GRAFO_INCLUDE
#define _GRAFO_INCLUDE

#define _MAX_NOS_GRAFO 100
#define _MAX_ARCOS_GRAFO 200
#define _MAX_RAMPAS_GRAFO 100

#define _MAX_OBSTACULOS 50

#define NORTE_SUL	0
#define ESTE_OESTE	1
#define OBLIQUA	2
#define RAMPA	3
#define PLANO		2

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

typedef struct No{
	float x, y, z, largura;
}No;

typedef struct Arco{
	int noi, nof;
	float peso, largura;
}Arco;

typedef struct Paredes{
	float xi,yi;
	float xf, yf;
	float xi2 = -100, yi2 = -100;
	float xf2 = -100, yf2 = -100;
	float z;
	string tipo;
}Paredes;

typedef struct Obstaculos {
	float xi, yi;
	float xf, yf;
}Obstaculos;

extern Paredes paredes[];
extern int numParedes;

extern Obstaculos obstaculos[];
extern int numObstaculos;

extern No nosMundo[];
extern Arco arcosMundo[];
extern int numNosMundo, numArcosMundo;

extern No nosQuarto[];
extern Arco arcosQuarto[];
extern int numNosQuarto, numArcosQuarto, numRampasQuarto;

extern No rampas[];

extern string mundoFile;
extern string quartoFile;

extern int numLuzes;
extern No nosLuz[];

extern int tamanhoQuartoX;
extern int tamanhoQuartoY;

extern int quantidadeInimigosVisiveis;
extern int posxInimigos[100];
extern int posyInimigos[100];
extern int poszInimigos[100];

extern int quantidadeAlcapoes;
extern int posxalcapao[30];
extern int posyalcapao[30];
extern int poszalcapao[30];

extern int quantidadeAlcapoesQuartoSecreto;
extern int posxalcapaoQuartoSecreto[30];
extern int posyalcapaoQuartoSecreto[30];
extern int poszalcapaoQuartoSecreto[30];
extern  int posxfinaisAlcapao[30];
extern int posyfinaisAlcapao[30];

extern int numPosicaoTrans;
extern double posxtrans[30];
extern double posytrans[30];
extern double porta[30];
extern int posxfinais[30];
extern int posyfinais[30];
extern int proxquarto[30];

extern int posxfinal[30];
extern int posyfinal[30];
extern int posxinicio[30];
extern int posyinicio[30];

extern int quantidadeObstaculos;
extern int posXObstaculos[50];
extern int posYObstaculos[50];

extern int pontuacao;

No criaNo(float, float, float);
void addNoMundo(No);
void addNoQuarto(No);

Arco criaArco(int, int, float, float);
void addArcoMundo(Arco);
void addArcoQuarto(Arco);

void setFicheiroMundo(string nome);
void setFicheiroQuarto(string nome);

void leGrafoMundo(string mundo);
void leGrafoQuarto(string quarto);
vector<string> split(string str, char delimiter);
float converter(string num);
#endif