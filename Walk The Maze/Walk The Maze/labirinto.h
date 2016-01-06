#ifndef _LABIRINTO_INCLUDE
#define _LABIRINTO_INCLUDE

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <AL/alut.h>
#include "grafos.h"
#include "structs.h"

using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef _WIN32
#include <GL/glaux.h>
#endif

extern int flag;

extern Porta portas;

extern Barril barris;

extern FimMundo fimMundo;

void material(enum tipo_material mat);

void desenhaSolo(GLuint texID);

GLfloat InnerProduct(GLfloat v[], GLfloat q[]);

void CrossProduct(GLdouble v1[], GLdouble v2[], GLdouble cross[]);

GLdouble VectorNormalize(GLdouble v[]);

void desenhaNormal(GLdouble x, GLdouble y, GLdouble z, GLdouble normal[], tipo_material mat);

void desenhaParede(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, GLuint texID, Estado estado);

void desenhaChao(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, int orient, Estado estado);

void desenhaNo(int no, Arco arcos[], No nos[], int numArcos, Modelo modelo, Estado estado);

void desenhaArco(Arco arco, No nos[], Modelo modelo, Estado estado);

void desenhaLabirinto(int numNos, No nos[], int numArcos, Arco arcos[], int numRampas, No rampas[], Modelo modelo, Estado estado);

void desenhaRampas(No rampa, No nos[], int numNos, Modelo modelo, Estado estado);

void desenhaCubo(float tamanho, GLfloat x, GLfloat y, int i);

void desenhaObstaculos(Menu menu, int quantidadeObstaculos, int posXObstaculos[], int posYObstaculos[]);

void desenhaBarril(GLdouble nx, GLdouble ny, int i);

#endif