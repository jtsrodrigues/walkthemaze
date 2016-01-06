#define _USE_MATH_DEFINES
#include<iostream>
#include "labirinto.h"
#include "callServices.h"
#include "timer.h"

#define CURL_STATICLIB
#define STATIC_LIBCURL
#include <curl\curl.h>

Estado estado;
Modelo modelo;
ModeloInimigo modeloInimigo[100];
boolean inAlcapao = false;
Menu menu;
Login login;
FimMapa fimMapa;
FimQuarto fimQuarto;
bool flagAnimacao = false;
GLboolean loadActivo = GL_FALSE;
GLboolean jogoActivo = GL_FALSE;
boolean verificaSeHomerEstaNoRange[100];
char nomeMDLjugador1[100] = "homer.mdl";
char nomeMDLinimigo1[100] = "Scream.mdl";
char nomeMDLjugador2[100] = "Scream.mdl";
char nomeMDLinimigo2[100] = "ronny.mdl";
char nomeMDLjugador3[100] = "ronny.mdl";
char nomeMDLinimigo3[100] = "homer.mdl";
string mdlactualjugador = "homer.mdl";
string mdlactualinimigo = "homer.mdl";
string sameMDL = "same";//no inicio o modelo inimigo e o outro sao o mesmo 
int mundoAtual;
float resolucaoX;
float resolucaoY;

////////////////////////////////////////////////////camara mini mapa
typedef struct GraphCamera{
	GLfloat height;
	GLdouble dir_lat;
	GLdouble dir_long;
	GLfloat dist;
	GLfloat vel;
	Vertice eye;
	Vertice center;
}GraphCamera;

typedef struct GraphWindows{
	GLint        Main;
	GLint        Top;
}GraphWindows;

GraphWindows Window;
GraphCamera MinimapCam;
////////////////////////////////////////////////////////////////////


///////////////////////////Alint de sons
ALint stateExplosao;
ALint stateDonut;
////////////////////////////////////////

//////////////////////////////////////////////////////////////Particulas Explosao
#define NUM_PARTICLES    1000          /* Number of particles  */

/* A particle */

struct particleData
{
	float   position[3];
	float   speed[3];
	float   color[3];
};
typedef struct particleData    particleData;


/* Globals */

particleData     particles[NUM_PARTICLES];
//debrisData       debris[NUM_DEBRIS];
int              fuel = 0;                /* "fuel" of the explosion */
int decaimento = 0;

float            angle = 0.0;             /* camera rotation angle */

int      wantNormalize = 1;   /* Speed vector normalization flag */
int      wantPause = 0;       /* Pause flag */
////////////////////////////////////////////////////////////////////////////////

#pragma comment (lib, "glaux.lib")    /* link with Win32 GLAUX lib */
#pragma comment( user, "Compiled on " __DATE__ " at " __TIME__ ) 

extern "C" int read_JPEG_file(char *, char **, int *, int *, int *);

#ifdef _WIN32
#define NOME_TEXTURA_CUBOS        "Textura.bmp"
#else
#define NOME_TEXTURA_CUBOS        "Textura.jpg"
#endif
//#define NOME_TEXTURA_CHAO         "Chao.jpg"
string nomeTexturaChao = "Chao.jpg";
//string nomeTexturaChao = "c"

#define NUM_TEXTURAS              2
#define ID_TEXTURA_CUBOS          0
#define ID_TEXTURA_CHAO           1

#define GRAUS(x)        (180*(x)/M_PI)

using namespace std;

bool detetaColisoes = true;
string vista_atual = "vista_superior_inclinada";

string estadoDoTempo = "null";
bool chegouFimMundo = false;
bool chegouFim = false;
int quartoAtual = 0; //POS 0 = Quarto 1
timer aTimer; //timer para calcular pontuacao

int podeSerAtacado = 1; //se for maior que zero pode ser atacado

bool flagJogadorMorto = false;

#define	OBJECTO_ALTURA		  0.4
#define OBJECTO_VELOCIDADE	0.5
#define OBJECTO_ROTACAO		  2
#define OBJECTO_RAIO		    0.12
#define SCALE_HOMER         0.005*8
#define EYE_ROTACAO			    1

#define OBJECTO_VELOCIDADEINMIGO	0.2
#define RangeParaInimigoDetetarHomer 20





bool flagBateuNaParede = false; //para que o som de bater na parede nao fique continuo

#define graus(X) (double)((X)*180/M_PI)
#define rad(X)   (double)((X)*M_PI/180)

//define particulas
#define MAX_PARTICLES 1000
#define CLEAR	0
#define CHUVA	1
#define NEVE	2
#define	GRANIZO	3

particulas sistemaParticulas[MAX_PARTICLES];

void initLogin()
{
	login.loginActivo = GL_TRUE;
	login.nomeUtilizador = "";
	login.password = "";
}

void initFimNivel()
{
	fimMapa.FimMundoActivo = GL_FALSE;
	//fimMapa.pontuacao = 5000;
	fimMapa.FimQuarto = GL_FALSE;
}

void initMenu()
{
	menu.menuActivo = GL_FALSE;

	string infoMapas = callService_InfoMapa();
	string a = "";
	vector<string>mapas = split(infoMapas, '!');
	for (int i = 0; i < mapas.size(); i++)
	{
		vector<string>infoDeMapa = split(mapas[i], '|');
		menu.mundos[i] = infoDeMapa[0];
		menu.fichMundos[i] = infoDeMapa[1];
		for (int j = 2; j < infoDeMapa.size(); j++)
		{
			menu.quartoDeMundo[i][j - 2] = infoDeMapa[j];
		}

	}
	menu.mundoSelecionado = menu.mundos[0];
	menu.totalMundos = mapas.size();
	/*menu.mundos[0] = "Mundo 1";
	menu.mundos[1] = "Mundo 2";
	menu.mundoSelecionado = menu.mundos[0];
	menu.fichMundos[0] = "mundo1_limites.txt";
	menu.fichMundos[1] = "mundo2_limites.txt";
	menu.quartoDeMundo[0][0] = "mundo1_paredes.txt";
	menu.quartoDeMundo[1][0] = "mundo2_quarto1.txt";
	menu.quartoDeMundo[1][1] = "mundo2_quarto2.txt";
	menu.quartoDeMundo[1][2] = "mundo2_quarto3.txt";*/
}


void initEstado(){
	estado.camera.dir_lat = M_PI / 4;
	estado.camera.dir_long = 0;
	estado.camera.fov = 60;
	estado.camera.dist = 100;

	estado.eixo[0] = 0;
	estado.eixo[1] = 0;
	estado.eixo[2] = 0;
	estado.camera.center[0] = 0;
	estado.camera.center[1] = 0;
	estado.camera.center[2] = 0;
	estado.light = GL_TRUE;
	estado.apresentaNormais = GL_FALSE;
	estado.lightViewer = 1;
	estado.timer = 100;
	estado.delay = 1;

	//efeito dia/noite
	estado.noite = GL_FALSE;

	//estado das teclas
	estado.teclas.up = false;
	estado.teclas.down = false;
	estado.teclas.left = false;
	estado.teclas.right = false;
	estado.teclas.m = false;
	estado.teclas.n = false;

	estado.mouseState = GLUT_UP;
}

void initModelo(){
	modelo.escala = 0.2;

	modelo.cor_cubo = brass;
	modelo.g_pos_luz1[0] = -5.0;
	modelo.g_pos_luz1[1] = 5.0;
	modelo.g_pos_luz1[2] = 5.0;
	modelo.g_pos_luz1[3] = 1.0;
	modelo.g_pos_luz2[0] = 5.0;
	modelo.g_pos_luz2[1] = -15.0;
	modelo.g_pos_luz2[2] = 5.0;
	modelo.g_pos_luz2[3] = 1.0;

	//variaveis homer 
	modelo.andar = false,
		modelo.prev = false;
	//homer
	if (fimMapa.FimQuarto){
		modelo.objecto.pos.x = (fimQuarto.posinicio) * 5;
		modelo.objecto.pos.y = (fimQuarto.posfim) * 5;
		fimQuarto.FimQuartoActivo = GL_FALSE;
		modelo.objecto.posicoes = "";
	}
	else{
		modelo.objecto.pos.x = posxinicio[0] * 5;
		modelo.objecto.pos.y = posyinicio[0] * 5;
	}

	modelo.objecto.pos.z = OBJECTO_ALTURA + 1.1;
	modelo.objecto.dir = 0;
	modelo.objecto.vel = OBJECTO_VELOCIDADE;
	modelo.objecto.vida = 105;
	modelo.andar = GL_FALSE;
	modelo.vida = 3;

}
void initModeloInimigo(){
	for (size_t i = 0; i < quantidadeInimigosVisiveis; i++)
	{




		modeloInimigo[i].escala = 0.2;

		modeloInimigo[i].cor_cubo = brass;
		modeloInimigo[i].g_pos_luz1[0] = -5.0;
		modeloInimigo[i].g_pos_luz1[1] = 5.0;
		modeloInimigo[i].g_pos_luz1[2] = 5.0;
		modeloInimigo[i].g_pos_luz1[3] = 1.0;
		modeloInimigo[i].g_pos_luz2[0] = 5.0;
		modeloInimigo[i].g_pos_luz2[1] = -15.0;
		modeloInimigo[i].g_pos_luz2[2] = 5.0;
		modeloInimigo[i].g_pos_luz2[3] = 1.0;

		//variaveis homer inimigo
		modeloInimigo[i].andar = false;
		modeloInimigo[i].prev = false;
		//homer




		modeloInimigo[i].objecto.pos.x = posxInimigos[i] * 5;
		modeloInimigo[i].objecto.pos.y = posyInimigos[i] * 5;
		modeloInimigo[i].objecto.pos.z = poszInimigos[i] + OBJECTO_ALTURA + 1.1;
		modeloInimigo[i].objecto.vel = OBJECTO_VELOCIDADEINMIGO;


		modeloInimigo[i].objecto.dir = 0;



		modeloInimigo[i].objecto.posicoes = "";
		modeloInimigo[i].andar = GL_FALSE;

		modeloInimigo[i].vida = 1;
		modeloInimigo[i].visivel = true;


	}



}

char*     alBuffer;             //data for the buffer
ALenum alFormatBuffer;    //buffer format
ALsizei   alFreqBuffer;       //frequency
ALsizei      alBufferLen;        //bit depth
ALboolean    alLoop;

void InitAudio(string tipo)
{
	if (tipo == "musica_fundo")
	{
		alSourcei(estado.source, AL_BUFFER, AL_NONE);
		alSourceUnqueueBuffers(1, 1, &estado.buffer);
		alutLoadWAVFile("Musica.wav", &alFormatBuffer, (void **)&alBuffer, &alBufferLen, &alFreqBuffer, &alLoop);
		alBufferData(estado.buffer, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		alSourcei(estado.source, AL_BUFFER, estado.buffer);
		alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		estado.tecla_s = AL_FALSE;
	}
	else if (tipo == "chuva")
	{
		estado.bufferChuva = alutCreateBufferFromFile("Chuva.wav");
		alGenSources(1, &estado.sourceChuva);
		alSourcei(estado.sourceChuva, AL_BUFFER, estado.bufferChuva);
	}
	else if (tipo == "granizo")
	{
		estado.bufferGranizo = alutCreateBufferFromFile("Granizo.wav");
		alGenSources(1, &estado.sourceGranizo);
		alSourcei(estado.sourceGranizo, AL_BUFFER, estado.bufferGranizo);
	}
	else if (tipo == "neve")
	{
		estado.bufferNeve = alutCreateBufferFromFile("Neve.wav");
		alGenSources(1, &estado.sourceNeve);
		alSourcei(estado.sourceNeve, AL_BUFFER, estado.bufferNeve);
	}
	else if (tipo == "baterParede")
	{
		estado.bufferBaterParede = alutCreateBufferFromFile("BaterParede.wav");
		alGenSources(1, &estado.sourceBaterParede);
		alSourcei(estado.sourceBaterParede, AL_BUFFER, estado.bufferBaterParede);
	}
	else if (tipo == "disparoDonut")
	{
		alSourcei(estado.sourceDonut, AL_BUFFER, AL_NONE);
		alSourceUnqueueBuffers(1, 1, &estado.bufferDonut);
		alutLoadWAVFile("Donut.wav", &alFormatBuffer, (void **)&alBuffer, &alBufferLen, &alFreqBuffer, &alLoop);
		alBufferData(estado.bufferDonut, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		alSourcei(estado.sourceDonut, AL_BUFFER, estado.bufferDonut);
		alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		//estado.bufferDonut = alutCreateBufferFromFile("Donut.wav");
		//alGenSources(1, &estado.sourceDonut);
		//alSourcei(estado.sourceDonut, AL_BUFFER, estado.bufferDonut);
	}
	else if (tipo == "musica_fundo2")
	{
		alSourcei(estado.source, AL_BUFFER, AL_NONE);
		alSourceUnqueueBuffers(1, 1, &estado.buffer);
		alutLoadWAVFile("MusicaDownload.wav", &alFormatBuffer, (void **)&alBuffer, &alBufferLen, &alFreqBuffer, &alLoop);
		alBufferData(estado.buffer, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		alSourcei(estado.source, AL_BUFFER, estado.buffer);
		alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		estado.tecla_s = AL_FALSE;
	}
	else if (tipo == "explosao")
	{
		alSourcei(estado.sourceExplosao, AL_BUFFER, AL_NONE);
		alSourceUnqueueBuffers(1, 1, &estado.bufferExplosao);
		alutLoadWAVFile("explosao.wav", &alFormatBuffer, (void **)&alBuffer, &alBufferLen, &alFreqBuffer, &alLoop);
		alBufferData(estado.bufferExplosao, alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		alSourcei(estado.sourceExplosao, AL_BUFFER, estado.bufferExplosao);
		alutUnloadWAV(alFormatBuffer, alBuffer, alBufferLen, alFreqBuffer);
		//estado.bufferExplosao = alutCreateBufferFromFile("explosao.wav");
		//alGenSources(1, &estado.sourceExplosao);
		//alSourcei(estado.sourceExplosao, AL_BUFFER, estado.bufferExplosao);
	}
}

void initParticulas(int i)
{
	sistemaParticulas[i].viva = true;
	sistemaParticulas[i].vida = 1.0;
	sistemaParticulas[i].desaparecimento = float(rand() % 100) / 1000.0f + 0.003f;

	sistemaParticulas[i].x = (float)(rand() % (tamanhoQuartoX * 5)) - (tamanhoQuartoX)* 2.5;
	sistemaParticulas[i].y = (float)(rand() % (tamanhoQuartoY * 5)) - (tamanhoQuartoY)* 2.5;
	sistemaParticulas[i].z = 100;

	sistemaParticulas[i].vel = 0;
	sistemaParticulas[i].grav = -0.8;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void downloadMusica()
{
	CURL *pCurl;
	FILE *fptr;
	CURLcode codes;
	string urlServico = callService_DownloadMusica("Megaman XX");

	char* p = new char[urlServico.length() + 1];
	memcpy(p, urlServico.c_str(), urlServico.length() + 1);

	char *url = p;
	char outfilename[256] = "MusicaDownload.wav";
	pCurl = curl_easy_init();
	if (pCurl) {
		fptr = fopen(outfilename, "wb");
		curl_easy_setopt(pCurl, CURLOPT_URL, url);
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, fptr);
		curl_easy_perform(pCurl);
		curl_easy_cleanup(pCurl);
		fclose(fptr);
	}
}
void downloadModeloinimigo(string nome)
{
	CURL *pCurl;
	FILE *fptr;
	CURLcode codes;
	string urlServico = callService_DownloadModeloInimigo(nome);

	char* p = new char[urlServico.length() + 1];
	memcpy(p, urlServico.c_str(), urlServico.length() + 1);

	char *url = p;
	char outfilename[256] = "modeloinimigo.mdl";
	pCurl = curl_easy_init();
	if (pCurl) {
		fptr = fopen(outfilename, "wb");
		curl_easy_setopt(pCurl, CURLOPT_URL, url);
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, fptr);
		curl_easy_perform(pCurl);
		curl_easy_cleanup(pCurl);
		fclose(fptr);
	}
}


void downloadTextura()
{
	CURL *pCurl;
	FILE *fptr;
	CURLcode codes;
	string urlServico = callService_DownloadTextura("Chao2");

	char* p = new char[urlServico.length() + 1];
	memcpy(p, urlServico.c_str(), urlServico.length() + 1);

	char *url = p;
	char outfilename[256] = "Chao2.jpg";
	pCurl = curl_easy_init();
	if (pCurl) {
		fptr = fopen(outfilename, "wb");
		curl_easy_setopt(pCurl, CURLOPT_URL, url);
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, fptr);
		curl_easy_perform(pCurl);
		curl_easy_cleanup(pCurl);
		fclose(fptr);
	}
}

//Tirar o som quando nao há particulas a cair do céu, mas a tecla P está ativa (tempo "CLEAR")
void retiraSom()
{
	if (estadoDoTempo == "chuva")
	{
		ALint stateChuva;
		alGetSourcei(estado.sourceChuva, AL_SOURCE_STATE, &stateChuva);
		if (stateChuva == AL_PLAYING) {
			alSourceStop(estado.sourceChuva);
			estadoDoTempo = "null";
		}
	}
	else if (estadoDoTempo == "granizo")
	{
		ALint stateGranizo;
		alGetSourcei(estado.sourceGranizo, AL_SOURCE_STATE, &stateGranizo);
		if (stateGranizo == AL_PLAYING) {
			alSourceStop(estado.sourceGranizo);
			estadoDoTempo = "null";
		}
	}
	else if (estadoDoTempo == "neve")
	{
		ALint stateNeve;
		alGetSourcei(estado.sourceNeve, AL_SOURCE_STATE, &stateNeve);
		if (stateNeve == AL_PLAYING){
			alSourceStop(estado.sourceNeve);
			estadoDoTempo = "null";
		}
	}
}

// Chuva
void desenhaChuva() {
	if (estadoDoTempo != "chuva") {
		/* Desativar outros sons */
		if (estadoDoTempo == "granizo")
		{
			ALint stateGranizo;
			alGetSourcei(estado.sourceGranizo, AL_SOURCE_STATE, &stateGranizo);
			if (stateGranizo == AL_PLAYING) {
				alSourceStop(estado.sourceGranizo);
			}
		}
		else if (estadoDoTempo == "neve")
		{
			ALint stateNeve;
			alGetSourcei(estado.sourceNeve, AL_SOURCE_STATE, &stateNeve);
			if (stateNeve == AL_PLAYING){
				alSourceStop(estado.sourceNeve);
			}
		}
		/* Ativar o som da chuva */
		estadoDoTempo = "chuva";
		ALint stateChuva;
		alGetSourcei(estado.sourceChuva, AL_SOURCE_STATE, &stateChuva);
		if (stateChuva != AL_PLAYING)
		{
			InitAudio("chuva");
			InitAudio("chuva"); //tem que ter esta linha duas vezes senao nao da som
			alSourcePlay(estado.sourceChuva);
		}
		else
			if (stateChuva == AL_PLAYING)
				alSourceStop(estado.sourceChuva);
	}

	float x, y, z;
	for (int i = 0; i < MAX_PARTICLES; i = i + 2) {
		if (sistemaParticulas[i].viva == true) {
			x = sistemaParticulas[i].x;
			y = sistemaParticulas[i].y;
			z = sistemaParticulas[i].z;

			// Desenha Particulas
			GLfloat comprimento = float(rand() % 5) / 10;
			glColor3f(0.5, 0.5, 1.0);
			glBegin(GL_LINES);
			glVertex3f(x, y, z);
			glVertex3f(x, y, z + comprimento);
			glEnd();

			// Update de valores da particula
			//Movimentação da particula
			sistemaParticulas[i].z += sistemaParticulas[i].vel / (0.2 * 500);
			sistemaParticulas[i].vel += sistemaParticulas[i].grav;
			// Desaparecimento de particula
			sistemaParticulas[i].vida -= sistemaParticulas[i].desaparecimento;

			if (sistemaParticulas[i].z <= 0) {
				sistemaParticulas[i].vida = -1.0;
			}
			//Revive particula
			if (sistemaParticulas[i].vida < 0.0) {
				initParticulas(i);
			}
		}
	}
}

// Desenha Granizo
void desenhaGranizo() {
	if (estadoDoTempo != "granizo") {
		/* Desativar outros sons */
		if (estadoDoTempo == "chuva")
		{
			ALint stateChuva;
			alGetSourcei(estado.sourceChuva, AL_SOURCE_STATE, &stateChuva);
			if (stateChuva == AL_PLAYING) {
				alSourceStop(estado.sourceChuva);
			}
		}
		else if (estadoDoTempo == "neve")
		{
			ALint stateNeve;
			alGetSourcei(estado.sourceNeve, AL_SOURCE_STATE, &stateNeve);
			if (stateNeve == AL_PLAYING){
				alSourceStop(estado.sourceNeve);
			}
		}

		/* Ativar o som do granizo */
		estadoDoTempo = "granizo";
		ALint stateGranizo;
		alGetSourcei(estado.sourceGranizo, AL_SOURCE_STATE, &stateGranizo);
		if (stateGranizo != AL_PLAYING)
		{
			InitAudio("granizo");
			InitAudio("granizo"); //tem que ter esta linha duas vezes senao nao da som
			alSourcePlay(estado.sourceGranizo);
		}
		else
			if (stateGranizo == AL_PLAYING)
				alSourceStop(estado.sourceGranizo);
	}

	float x, y, z;
	for (int i = 0; i < MAX_PARTICLES; i = i + 2) {
		if (sistemaParticulas[i].viva == true) {
			x = sistemaParticulas[i].x;
			y = sistemaParticulas[i].y;
			z = sistemaParticulas[i].z;

			// Desenha Particulas
			float hailsize = 0.05;
			glColor3f(0.8, 0.8, 0.9);
			glBegin(GL_QUADS);
			// Front
			glVertex3f(x - hailsize, y - hailsize, z + hailsize); // lower left
			glVertex3f(x - hailsize, y + hailsize, z + hailsize); // upper left
			glVertex3f(x + hailsize, y + hailsize, z + hailsize); // upper right
			glVertex3f(x + hailsize, y - hailsize, z + hailsize); // lower left
			//Left
			glVertex3f(x - hailsize, y - hailsize, z + hailsize);
			glVertex3f(x - hailsize, y - hailsize, z - hailsize);
			glVertex3f(x - hailsize, y + hailsize, z - hailsize);
			glVertex3f(x - hailsize, y + hailsize, z + hailsize);
			// Back
			glVertex3f(x - hailsize, y - hailsize, z - hailsize);
			glVertex3f(x - hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z - hailsize);
			//Right
			glVertex3f(x + hailsize, y + hailsize, z + hailsize);
			glVertex3f(x + hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z + hailsize);
			//Top 
			glVertex3f(x - hailsize, y + hailsize, z + hailsize);
			glVertex3f(x - hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y + hailsize, z + hailsize);
			//Bottom 
			glVertex3f(x - hailsize, y - hailsize, z + hailsize);
			glVertex3f(x - hailsize, y - hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z + hailsize);
			glEnd();

			// Update de valores da particula
			//Movimentação da particula
			sistemaParticulas[i].z += sistemaParticulas[i].vel / (0.2 * 1000);
			sistemaParticulas[i].vel += sistemaParticulas[i].grav;
			// Desaparecimento de particula
			sistemaParticulas[i].vida -= sistemaParticulas[i].desaparecimento;

			if (sistemaParticulas[i].z <= 0) {
				sistemaParticulas[i].vida = -1.0;
			}
			//Revive particula
			if (sistemaParticulas[i].vida < 0.0) {
				initParticulas(i);
			}
		}
	}
}

// Neve
void desenhaNeve() {
	if (estadoDoTempo != "neve") {
		/* Desativar outros sons */
		if (estadoDoTempo == "chuva")
		{
			ALint stateChuva;
			alGetSourcei(estado.sourceChuva, AL_SOURCE_STATE, &stateChuva);
			if (stateChuva == AL_PLAYING) {
				alSourceStop(estado.sourceChuva);
			}
		}
		else if (estadoDoTempo == "granizo")
		{
			ALint stateGranizo;
			alGetSourcei(estado.sourceGranizo, AL_SOURCE_STATE, &stateGranizo);
			if (stateGranizo == AL_PLAYING){
				alSourceStop(estado.sourceGranizo);
			}
		}

		/* Ativa o som da chuva */
		estadoDoTempo = "neve";
		ALint stateNeve;
		alGetSourcei(estado.sourceNeve, AL_SOURCE_STATE, &stateNeve);
		if (stateNeve != AL_PLAYING)
		{
			InitAudio("neve");
			InitAudio("neve"); //tem que ter esta linha duas vezes senao nao da som
			alSourcePlay(estado.sourceNeve);
		}
		else
			if (stateNeve == AL_PLAYING)
				alSourceStop(estado.sourceNeve);
	}

	float x, y, z;
	for (int i = 0; i < MAX_PARTICLES; i = i + 2) {
		if (sistemaParticulas[i].viva == true) {
			x = sistemaParticulas[i].x;
			y = sistemaParticulas[i].y;
			z = sistemaParticulas[i].z;

			// Desenha particulas
			glColor3f(1.0, 1.0, 1.0);
			glPushMatrix();
			glTranslatef(x, y, z);
			glutSolidSphere(0.1, 16, 16);
			glPopMatrix();

			// Update de valores da particula
			//Movimentação da particula

			GLfloat oscilacao = float(-2 + (rand() % 5)) / 10;
			sistemaParticulas[i].x += oscilacao;
			oscilacao = float(-2 + (rand() % 5)) / 10;
			sistemaParticulas[i].y += oscilacao;
			sistemaParticulas[i].z += sistemaParticulas[i].vel / (0.2 * 2000);
			sistemaParticulas[i].vel += sistemaParticulas[i].grav;
			// Desaparecimento de particula
			sistemaParticulas[i].vida -= sistemaParticulas[i].desaparecimento;

			if (sistemaParticulas[i].z <= 0) {
				sistemaParticulas[i].vida = -1.0;
			}

			//Revive particula
			if (sistemaParticulas[i].vida < 0.0) {
				initParticulas(i);
			}
		}
	}
}

void myInit()
{

	GLfloat LuzAmbiente[] = { 0.5, 0.5, 0.5, 0.0 };

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_DEPTH_TEST); /* enable z buffer */
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);

	glDepthFunc(GL_LESS);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, estado.lightViewer);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	//depois de implementado os webservices não é preciso os ficheiros locais
	//setFicheiroMundo("mundo1_limites.txt");
	//setFicheiroQuarto("mundo1_paredes.txt");

	/*leGrafoMundo(callService_DownloadMapa("mundo1_limites.txt"));
	leGrafoQuarto(callService_DownloadMapa("mundo1_paredes.txt"));*/

	//initModelo();
	//initModeloInimigo();
	initEstado();



	//modelo.quad = gluNewQuadric();
	//gluQuadricDrawStyle(modelo.quad, GLU_FILL);





	/*

		gluQuadricNormals(modelo.quad, GLU_OUTSIDE);

		for (int i = 0; i < quantidadeInimigosVisiveis; i++)
		{
		modeloInimigo[i].quad = gluNewQuadric();
		gluQuadricDrawStyle(modeloInimigo[i].quad, GLU_FILL);
		gluQuadricNormals(modeloInimigo[i].quad, GLU_OUTSIDE);
		gluQuadricDrawStyle(modeloInimigo[i].quad, GLU_FILL);
		gluQuadricNormals(modeloInimigo[i].quad, GLU_OUTSIDE);
		}

		*/

	//inicia particulas
	/*for (int i = 0; i < MAX_PARTICLES; i++)
	{
	initParticulas(i);

	}*/
}


void imprime_ajuda(void)
{
	printf("\n\nDesenho de um labirinto a partir de um grafo\n");
	printf("h,H - Ajuda \n");
	printf("c,C - Altera camera \n");
	printf("z,Z - Desativa/Ativa Colisoes \n");
	printf("space - Atirar donut \n");
	printf("ESC - Sair\n");
}


const GLfloat red_light[] = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat green_light[] = { 0.0, 1.0, 0.0, 1.0 };
const GLfloat blue_light[] = { 0.0, 0.0, 1.0, 1.0 };
const GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };


void putLights(GLfloat* diffuse)
{
	const GLfloat white_amb[] = { 0.2, 0.2, 0.2, 1.0 };
	unsigned int luzes[] = { GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };
	const GLfloat spot_dir[] = { 0.0, 0.0, -1.0 };

	GLfloat pos0[] = { 0, 0, 120, 0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_dir);

	if (estado.noite){
		for (int i = 0; i < numLuzes; i++)

		{
			glPushMatrix();
			glScalef(5, 5, 5);
			GLfloat posicao[] = { nosLuz[i].x, nosLuz[i].y, nosLuz[i].z, nosLuz[i].largura };
			glLightfv(luzes[i], GL_DIFFUSE, diffuse);
			glLightfv(luzes[i], GL_SPECULAR, white_light);
			glLightfv(luzes[i], GL_AMBIENT, white_amb);
			glLightfv(luzes[i], GL_POSITION, posicao);
			glLightfv(luzes[i], GL_SPOT_DIRECTION, spot_dir);
			glLightf(luzes[i], GL_SPOT_CUTOFF, 90.0f);

			/*desenha luz*/
			material(red_plastic);
			glPushMatrix();
			/*glScalef(5, 5, 5);*/
			glTranslatef(posicao[0], posicao[1], posicao[2]);
			glDisable(GL_LIGHTING);
			glColor3f(1.0, 1.0, 1.0);
			glutSolidCube(0.2);
			glEnable(GL_LIGHTING);
			glPopMatrix();

			glEnable(luzes[i]);
			glPopMatrix();

		}
	}
	else{
		for (int i = 0; i < numLuzes; i++)
		{
			glDisable(luzes[i]);
		}
	}

	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, white_amb);
	//glLightfv(GL_LIGHT0, GL_POSITION, modelo.g_pos_luz1);

	//glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, white_amb);
	//glLightfv(GL_LIGHT1, GL_POSITION, modelo.g_pos_luz2);

	///* desenhar luz */
	//material(red_plastic);
	//glPushMatrix();
	//	glTranslatef(modelo.g_pos_luz1[0], modelo.g_pos_luz1[1], modelo.g_pos_luz1[2]);
	//	glDisable(GL_LIGHTING);
	//	glColor3f(1.0, 1.0, 1.0);
	//	glutSolidCube(0.2);
	//	glEnable(GL_LIGHTING);
	//glPopMatrix();
	//glPushMatrix();
	//	glTranslatef(modelo.g_pos_luz2[0], modelo.g_pos_luz2[1], modelo.g_pos_luz2[2]);
	//	glDisable(GL_LIGHTING);
	//	glColor3f(1.0, 1.0, 1.0);
	//	glutSolidCube(0.2);
	//	glEnable(GL_LIGHTING);
	//glPopMatrix();

	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
}


#define EIXO_X		1
#define EIXO_Y		2
#define EIXO_Z		3

void desenhaPlanoDrag(int eixo){
	glPushMatrix();
	glTranslated(estado.eixo[0], estado.eixo[1], estado.eixo[2]);
	switch (eixo) {
	case EIXO_Y:
		if (abs(estado.camera.dir_lat) < M_PI / 4)
			glRotatef(-90, 0, 0, 1);
		else
			glRotatef(90, 1, 0, 0);
		material(red_plastic);
		break;
	case EIXO_X:
		if (abs(estado.camera.dir_lat) > M_PI / 6)
			glRotatef(90, 1, 0, 0);
		material(azul);
		break;
	case EIXO_Z:
		if (abs(cos(estado.camera.dir_long)) > 0.5)
			glRotatef(90, 0, 0, 1);

		material(emerald);
		break;
	}
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-100, 0, -100);
	glVertex3f(100, 0, -100);
	glVertex3f(100, 0, 100);
	glVertex3f(-100, 0, 100);
	glEnd();
	glPopMatrix();
}

void desenhaEixos(){

	glPushMatrix();
	glTranslated(estado.eixo[0], estado.eixo[1], estado.eixo[2]);
	material(emerald);
	glPushName(EIXO_Z);
	//desenhaEixo();
	glPopName();
	glPushName(EIXO_Y);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	material(red_plastic);
	//desenhaEixo();
	glPopMatrix();
	glPopName();
	glPushName(EIXO_X);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	material(azul);
	//desenhaEixo();
	glPopMatrix();
	glPopName();
	glPopMatrix();
}

void redisplayAll(void)
{
	glutSetWindow(Window.Main);
	glutPostRedisplay();
	glutSetWindow(Window.Top);
	glutPostRedisplay();
}

void TopCamLookAt(){
	MinimapCam.eye[0] = modelo.objecto.pos.x;
	MinimapCam.eye[1] = modelo.objecto.pos.y;
	MinimapCam.eye[2] = 100;

	gluLookAt(MinimapCam.eye[0], MinimapCam.eye[1], 100, estado.camera.center[0], estado.camera.center[1], estado.camera.center[2],
		cos(estado.camera.dir_long), sin(estado.camera.dir_long), 0);

}

void billboardCylindricalBegin(
	float camX, float camY, float camZ,
	float objPosX, float objPosY, float objPosZ) {

	float lookAt[3], objToCamProj[3], upAux[3];
	float modelview[16], angleCosine;

	glPushMatrix();

	objToCamProj[0] = camX - objPosX;
	objToCamProj[1] = camY - objPosY;
	objToCamProj[2] = 0;


	lookAt[0] = 1;
	lookAt[1] = 0;
	lookAt[2] = 0;

	VectorNormalize(objToCamProj);


	CrossProduct(lookAt, objToCamProj, upAux);

	angleCosine = InnerProduct(lookAt, objToCamProj);

	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		glRotatef(acos(angleCosine) * 180 / 3.14, upAux[0], upAux[1], upAux[2]);
}

void billboardEnd() {

	glPopMatrix();
}

void setCamera(){


	if (vista_atual == "terceira_pessoa"){ //camera 3
		//cout << "X: " << modelo.objecto.pos.x << " || Y: " << modelo.objecto.pos.y << endl;
		//cout << "Tlong: " << estado.camera.dir_long << "lat: " << estado.camera.dir_lat << endl;



		pos_t center;
		estado.camera.center[0] = modelo.objecto.pos.x;
		estado.camera.center[1] = modelo.objecto.pos.y;
		estado.camera.center[2] = modelo.objecto.pos.z + 0.5;

		estado.camera.eye.x = estado.camera.center[0] - 5 * cos(estado.camera.dir_long);
		estado.camera.eye.y = estado.camera.center[1] - 5 * sin(estado.camera.dir_long);
		estado.camera.eye.z = estado.camera.center[2] + 0.5;


		if (estado.light){
			gluLookAt(estado.camera.eye.x, estado.camera.eye.y, estado.camera.eye.z, estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], 0, 0, 1);
			putLights((GLfloat*)white_light);
		}
		else{
			putLights((GLfloat*)white_light);
			gluLookAt(estado.camera.eye.x, estado.camera.eye.y, estado.camera.eye.z, estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], 0, 0, 1);
		}
	}


	else if (vista_atual == "primeira_pessoa"){//vista para a 1 pessoa
		//cout << "Plong: " << estado.camera.dir_long << "lat: " << estado.camera.dir_lat << endl;
		pos_t center;
		estado.camera.eye.x = modelo.objecto.pos.x;
		estado.camera.eye.y = modelo.objecto.pos.y;
		estado.camera.eye.z = modelo.objecto.pos.z + 0.5;

		center.x = modelo.objecto.pos.x + sin(-estado.camera.dir_long)*cos(estado.camera.dir_lat);
		center.y = modelo.objecto.pos.y + cos(estado.camera.dir_long)*cos(estado.camera.dir_lat);
		center.z = estado.camera.eye.z;

		//cout << "X " << center.x << " Y " << estado.camera.eye.y << " Z " << center.z << endl;

		glRotatef(90.0, 0.0, 1.0, 0.0);
		if (estado.light){
			gluLookAt(estado.camera.eye.x, estado.camera.eye.y, estado.camera.eye.z, center.x, center.y, center.z, 0, 0, 1);
			putLights((GLfloat*)white_light);
		}
		else{
			putLights((GLfloat*)white_light);
			gluLookAt(estado.camera.eye.x, estado.camera.eye.y, estado.camera.eye.z, center.x, center.y, center.z, 0, 0, 1);
		}
	}



	else if (vista_atual == "vista_superior" || vista_atual == "vista_superior_inclinada"){
		//cout << "Slong: " << estado.camera.dir_long << "lat: " << estado.camera.dir_lat << endl;
		//cout << "long: " << estado.camera.dir_long << "lat: " << estado.camera.dir_lat << endl;
		estado.camera.center[0] = modelo.objecto.pos.x;
		estado.camera.center[1] = modelo.objecto.pos.y;
		estado.camera.center[2] = modelo.objecto.pos.z + 0.5;
		double lim = M_PI / 2 - 0.1;
		Vertice eye;
		eye[0] = estado.camera.center[0] + 100 * cos(estado.camera.dir_long)*cos(estado.camera.dir_lat);
		eye[1] = estado.camera.center[1] + 100 * sin(estado.camera.dir_long)*cos(estado.camera.dir_lat);
		eye[2] = estado.camera.center[2] + 100 * sin(estado.camera.dir_lat);

		estado.camera.eye.x = eye[0];
		estado.camera.eye.y = eye[1];
		estado.camera.eye.z = eye[2];

		if (estado.light){
			gluLookAt(eye[0], eye[1], eye[2], estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], 0, 0, 1);
			putLights((GLfloat*)white_light);
		}
		else{
			putLights((GLfloat*)white_light);
			gluLookAt(eye[0], eye[1], eye[2], estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], 0, 0, 1);
		}

	}

}



void desenhaBillBoard(ModeloInimigo modeloini)
{
	glPushMatrix();
	glTranslatef(modeloini.objecto.pos.x, modeloini.objecto.pos.y, modeloini.objecto.pos.z + 5);
	billboardCylindricalBegin(estado.camera.eye.x, estado.camera.eye.y, estado.camera.eye.z, modeloini.objecto.pos.x, modeloini.objecto.pos.y, modeloini.objecto.pos.z + 5);
	//billboardCheatCylindricalBegin();
	glPushMatrix();
	//material(brass);
	float comp = (float)modeloini.vida / (float)2;
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	glVertex3f(0, -comp, -0.25);
	glVertex3f(0, comp, -0.25);
	glVertex3f(0, comp, 0.25);
	glVertex3f(0, -comp, 0.25);
	/*glVertex3f(modelo.objecto.pos.x, modelo.objecto.pos.y - 2, modelo.objecto.pos.z + 3);
	glVertex3f(modelo.objecto.pos.x, modelo.objecto.pos.y - 2, modelo.objecto.pos.z + 3.5);
	glVertex3f(modelo.objecto.pos.x, modelo.objecto.pos.y + 2, modelo.objecto.pos.z + 3.5);
	glVertex3f(modelo.objecto.pos.x, modelo.objecto.pos.y + 2, modelo.objecto.pos.z + 3);*/
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
	billboardEnd();
	glPopMatrix();
}

bool dentroIntervalo(float x, float int1, float int2){
	if ((x > int1 && x<int2) || (x>int2 && x < int1)){
		return true;
	}
	return false;
}

bool colisao(GLfloat nx, GLfloat ny, string objeto){

	if (detetaColisoes == false) return false;
	int i;
	bool colisaoRampa = false, colisaoParede = false;
	for (i = 0; i < numParedes; i++){
		if (paredes[i].tipo == "rampaBaixo"){
			if (nx >= paredes[i].xi - 0.5 * 5 && nx <= paredes[i].xi + 0.5 * 5 && ny >= paredes[i].yi - (0.5 * 5) && ny <= paredes[i].yi + (0.5 * 5)){
				if (objeto == "donut"&& modelo.objecto.pos.z < 5) return true;
				else{
					modelo.objecto.pos.z = (paredes[i].yi + (0.5 * 5)) - modelo.objecto.pos.y + 1.8;
					colisaoRampa = true;
				}
			}
		}
		else
			if (paredes[i].tipo == "rampaCima"){
				if (dentroIntervalo(nx, paredes[i].xi - 0.5 * 5, paredes[i].xi + 0.5 * 5) && dentroIntervalo(ny, paredes[i].yi - (0.5 * 5), paredes[i].yi + (0.5 * 5))){

					if (objeto == "donut"&& modelo.objecto.pos.z < 5) return true;
					else{
						modelo.objecto.pos.z = modelo.objecto.pos.y - (paredes[i].yi - (0.5 * 5)) + 1.8;
						colisaoRampa = true;
					}
				}
			}
			else
				if (paredes[i].tipo == "rampaDireita"){
					if (ny >= paredes[i].yi - 0.5 * 5 && ny <= paredes[i].yi + 0.5 * 5 && nx >= paredes[i].xi - (0.5 * 5) && nx <= paredes[i].xi + (0.5 * 5)){

						if (objeto == "donut"&& modelo.objecto.pos.z < 5) return true;
						else{
							//modelo.objecto.pos.z = -(paredes[i].xi + (0.5 * 5) - modelo.objecto.pos.x) + 6.7;
							modelo.objecto.pos.z = modelo.objecto.pos.x - (paredes[i].xi - (0.5 * 5)) + 1.8;
							colisaoRampa = true;
						}
					}
				}
				else
					if (paredes[i].tipo == "rampaEsquerda"){
						if (ny >= paredes[i].yi - 0.5 * 5 && ny <= paredes[i].yi + 0.5 * 5 && nx >= paredes[i].xi - (0.5 * 5) && nx <= paredes[i].xi + (0.5 * 5)){

							if (objeto == "donut" && modelo.objecto.pos.z < 5) return true;
							else{
								modelo.objecto.pos.z = (paredes[i].xi + (0.5 * 5)) - modelo.objecto.pos.x + 1.8;
								//modelo.objecto.pos.z = (paredes[i].xi + (0.5 * 5) - modelo.objecto.pos.x) + 1.7;
								colisaoRampa = true;
							}
						}
					}
					else{


						if (paredes[i].tipo == "parede_reta"){
							//paredes retas
							//cout << "---- " << nx << ":" << paredes[i].xi << ";" << paredes[i].xf << "  " << ny << ":" << paredes[i].yi << ";" << paredes[i].yf << "            " << modelo.objecto.pos.z << endl;

							//if (modelo.objecto.pos.z < 5 && ((nx >= paredes[i].xi && nx <= paredes[i].xf && ny >= paredes[i].yi && ny <= paredes[i].yf) || (nx <= paredes[i].xi && nx >= paredes[i].xf && ny <= paredes[i].yi && ny >= paredes[i].yf))){
							//cout <<"z: "<< modelo.objecto.pos.z << endl;
							if (modelo.objecto.pos.z < 5 && ((ny >= paredes[i].yi && ny <= paredes[i].yf) || (ny >= paredes[i].yf && ny <= paredes[i].yi)) && ((nx >= paredes[i].xi && nx <= paredes[i].xf) || (nx >= paredes[i].xf && nx <= paredes[i].xi))){
								//modelo.homer.SetSequence(20);


								colisaoParede = true;
								if (flagBateuNaParede == false && objeto != "donut"){
									ALint stateBaterParede;
									alGetSourcei(estado.sourceBaterParede, AL_SOURCE_STATE, &stateBaterParede);
									if (stateBaterParede != AL_PLAYING) {
										InitAudio("baterParede");
										alSourcePlay(estado.sourceBaterParede);
										flagBateuNaParede = true;
									}
								}

								return true;
							}
							else{
								if (((ny >= paredes[i].yi && ny <= paredes[i].yf) || (ny >= paredes[i].yf && ny <= paredes[i].yi)) && ((nx >= paredes[i].xi && nx <= paredes[i].xf) || (nx >= paredes[i].xf && nx <= paredes[i].xi))){
									colisaoParede = true;
									modelo.objecto.pos.z = 6.5;
								}
							}
						}
						else{
							if (paredes[i].tipo == "parede_obliqua"){
								//obliquas



								float MaiorYreta1 = (((nx - paredes[i].xi)*(paredes[i].yf - paredes[i].yi)) / (paredes[i].xf - paredes[i].xi)) + (paredes[i].yi);
								float MaiorYreta2 = (((nx - paredes[i].xi2)*(paredes[i].yf2 - paredes[i].yi2)) / (paredes[i].xf2 - paredes[i].xi2)) + (paredes[i].yi2);

								float MaiorXreta1 = (((ny - paredes[i].yi)*(paredes[i].xf - paredes[i].xi)) / (paredes[i].yf - paredes[i].yi)) + (paredes[i].xi);
								float MaiorXreta2 = (((ny - paredes[i].yi2)*(paredes[i].xf2 - paredes[i].xi2)) / (paredes[i].yf2 - paredes[i].yi2)) + (paredes[i].xi2);


								float MenorYreta1 = (((nx - paredes[i].xi)*(paredes[i].yi2 - paredes[i].yi)) / (paredes[i].xi2 - paredes[i].xi)) + (paredes[i].yi);
								float MenorYreta2 = (((nx - paredes[i].xf)*(paredes[i].yf2 - paredes[i].yf)) / (paredes[i].xf2 - paredes[i].xf)) + (paredes[i].yf);

								float MenorXreta1 = (((ny - paredes[i].yi)*(paredes[i].xi2 - paredes[i].xi)) / (paredes[i].yi2 - paredes[i].yi)) + (paredes[i].xi);
								float MenorXreta2 = (((ny - paredes[i].yf)*(paredes[i].xf2 - paredes[i].xf)) / (paredes[i].yf2 - paredes[i].yf)) + (paredes[i].xf);



								if (modelo.objecto.pos.z < 5 && dentroIntervalo(nx, MaiorXreta1, MaiorXreta2) && dentroIntervalo(ny, MaiorYreta1, MaiorYreta2)
									&& dentroIntervalo(nx, MenorXreta1, MenorXreta2) && dentroIntervalo(ny, MenorYreta1, MenorYreta2)){
									//modelo.homer.SetSequence(20);
									colisaoParede = true;
									if (flagBateuNaParede == false && objeto != "donut"){
										ALint stateBaterParede;
										alGetSourcei(estado.sourceBaterParede, AL_SOURCE_STATE, &stateBaterParede);
										if (stateBaterParede != AL_PLAYING) {
											InitAudio("baterParede");
											alSourcePlay(estado.sourceBaterParede);
											flagBateuNaParede = true;
										}
									}
									//colisaoParede = true;
									return true;
								}else{
									if (modelo.objecto.pos.z > 5  && dentroIntervalo(nx, MaiorXreta1, MaiorXreta2) && dentroIntervalo(ny, MaiorYreta1, MaiorYreta2)
										&& dentroIntervalo(nx, MenorXreta1, MenorXreta2) && dentroIntervalo(ny, MenorYreta1, MenorYreta2)){
										colisaoParede = true;
										modelo.objecto.pos.z = 6.5;
									}
								}


								//cout << nx << " - " << x1 << " e " << x2 << endl;
								//cout << ny << " - " << paredes[i].yi << " e " << paredes[i].yf << endl << endl;


								/*if (modelo.objecto.pos.z < 5&& (nx >= paredes[i].xi && nx <= paredes[i].xf) && ((ny >= Yreta1 && ny <= Yreta2) || (ny >= Yreta2 && ny <= Yreta1))){
									//modelo.homer.SetSequence(20);
									colisaoParede = true;
									if (flagBateuNaParede == false && objeto != "donut"){
									ALint stateBaterParede;
									alGetSourcei(estado.sourceBaterParede, AL_SOURCE_STATE, &stateBaterParede);
									if (stateBaterParede != AL_PLAYING) {
									InitAudio("baterParede");
									alSourcePlay(estado.sourceBaterParede);
									flagBateuNaParede = true;
									}
									}
									//colisaoParede = true;
									return true;
									}*/
							}
						}
					}
					//cout << paredes[i].xi << " " << paredes[i].yi << endl;
					//cout << paredes[i].xf << " " << paredes[i].yf << endl << endl;


	}
	if (modelo.objecto.pos.z>3 && colisaoParede == false && colisaoRampa == false && objeto == "personagem"){
		//queda
		/*if (modelo.objecto.pos.z > 1.5){
			modelo.andar = GL_FALSE;
			modelo.objecto.pos.z -= 0.1;
			if (modelo.objecto.pos.z > 2)	modelo.objecto.vida -= 0.5;
		}
		else{
			modelo.andar = GL_TRUE;
			modelo.objecto.pos.z = 1.5;
		}*/
		modelo.objecto.pos.z = 1.5;
		modelo.objecto.vida -= 36;
	}
	for (i = 0; i < numObstaculos; i++) {
		if (nx >= obstaculos[i].xi && nx <= obstaculos[i].xf && ny >= obstaculos[i].yi && ny <= obstaculos[i].yf || nx <= obstaculos[i].xi && nx >= obstaculos[i].xf && ny <= obstaculos[i].yi && ny >= obstaculos[i].yf) {
			if (flagBateuNaParede == false && objeto != "donut"){
				ALint stateBaterParede;
				alGetSourcei(estado.sourceBaterParede, AL_SOURCE_STATE, &stateBaterParede);
				if (stateBaterParede != AL_PLAYING) {
					InitAudio("baterParede");
					alSourcePlay(estado.sourceBaterParede);
					flagBateuNaParede = true;
				}
			}
			return true;
		}
	}

	//Colisao dos donuts com os inimigos
	for (i = 0; i < quantidadeInimigosVisiveis; i++) {
		if (objeto == "donut") {
			//Formula a satisfazer: (x - centro_x)^2 + (y - centro_y)^2 < raio^2
			float centro_x = modeloInimigo[i].objecto.pos.x;
			float centro_y = modeloInimigo[i].objecto.pos.y;
			float raio = 1.7; //margem de erro estimada
			for (int j = 0; j < 10; j++) {
				if (modelo.objecto.donut[i].ativo) {
					float centroXDonut = modelo.objecto.donut[i].pos.x;
					float centroYDonut = modelo.objecto.donut[i].pos.y;
					if ((pow((centroXDonut - centro_x), 2) + pow((centroYDonut - centro_y), 2)) <= pow(raio, 2)) {
						modelo.objecto.donut[i].ativo = false;
						modeloInimigo[i].objecto.vida -= 1;
						modeloInimigo[i].vida -= 1;
						fimMapa.pontuacao += 100;
					}
				}
			}
		}
	}

	return false;


}



bool colisaoInimigo(GLfloat nx, GLfloat ny, string objeto, int j){
	int i;
	bool colisaoRampa = false, colisaoParede = false;

	for (i = 0; i < numParedes; i++){
		if (paredes[i].tipo == "rampaBaixo"){
			if (nx >= paredes[i].xi - 0.5 * 5 && nx <= paredes[i].xi + 0.5 * 5 && ny >= paredes[i].yi - (0.5 * 5) && ny <= paredes[i].yi + (0.5 * 5)){

				modeloInimigo[j].objecto.pos.z = (paredes[i].yi + (0.5 * 5) - modeloInimigo[j].objecto.pos.y) + 1.7;

				colisaoRampa = true;

			}
		}
		else
			if (paredes[i].tipo == "rampaCima"){
				if (nx >= paredes[i].xi - 0.5 * 5 && nx <= paredes[i].xi + 0.5 * 5 && ny >= paredes[i].yi - (0.5 * 5) && ny <= paredes[i].yi + (0.5 * 5)){

					modeloInimigo[j].objecto.pos.z = -(paredes[i].yi + (0.5 * 5) - modeloInimigo[j].objecto.pos.y) + 6.7;
					colisaoRampa = true;

				}
			}
			else
				if (paredes[i].tipo == "rampaDireita"){
					if (ny >= paredes[i].yi - 0.5 * 5 && ny <= paredes[i].yi + 0.5 * 5 && nx >= paredes[i].xi - (0.5 * 5) && nx <= paredes[i].xi + (0.5 * 5)){

						modeloInimigo[j].objecto.pos.z = -(paredes[i].xi + (0.5 * 5) - modeloInimigo[j].objecto.pos.x) + 6.7;
						colisaoRampa = true;

					}
				}
				else
					if (paredes[i].tipo == "rampaEsquerda"){
						if (ny >= paredes[i].yi - 0.5 * 5 && ny <= paredes[i].yi + 0.5 * 5 && nx >= paredes[i].xi - (0.5 * 5) && nx <= paredes[i].xi + (0.5 * 5)){

							modeloInimigo[j].objecto.pos.z = (paredes[i].xi + (0.5 * 5) - modeloInimigo[j].objecto.pos.x) + 1.7;
							colisaoRampa = true;

						}
					}
					else{


						if (paredes[i].tipo == "parede_reta"){

							if (modeloInimigo[j].objecto.pos.z < 5 && ((ny >= paredes[i].yi && ny <= paredes[i].yf) || (ny >= paredes[i].yf && ny <= paredes[i].yi)) && ((nx >= paredes[i].xi && nx <= paredes[i].xf) || (nx >= paredes[i].xf && nx <= paredes[i].xi))){



								colisaoParede = true;


								return true;
							}
							else{
								if (((ny >= paredes[i].yi && ny <= paredes[i].yf) || (ny >= paredes[i].yf && ny <= paredes[i].yi)) && ((nx >= paredes[i].xi && nx <= paredes[i].xf) || (nx >= paredes[i].xf && nx <= paredes[i].xi))){
									colisaoParede = true;
								}
							}
						}
						else{
							if (paredes[i].tipo == "parede_obliqua"){
								//obliquas



								float Yreta1 = (((nx - paredes[i].xi)*(paredes[i].yf - paredes[i].yi)) / (paredes[i].xf - paredes[i].xi)) + (paredes[i].yi);



								float Yreta2 = (((nx - paredes[i].xi2)*(paredes[i].yf2 - paredes[i].yi2)) / (paredes[i].xf2 - paredes[i].xi2)) + (paredes[i].yi2);





								if (modeloInimigo[j].objecto.pos.z < 5 && (nx >= paredes[i].xi && nx <= paredes[i].xf) && ((ny >= Yreta1 && ny <= Yreta2) || (ny >= Yreta2 && ny <= Yreta1))){
									//modelo.homer.SetSequence(20);
									colisaoParede = true;
									return true;
								}
							}
						}
					}
					//cout << paredes[i].xi << " " << paredes[i].yi << endl;
					//cout << paredes[i].xf << " " << paredes[i].yf << endl << endl;


	}
	if (colisaoParede == false && colisaoRampa == false && objeto == "personagem"){
		//queda
		if (modeloInimigo[j].objecto.pos.z > 1.5){
			modeloInimigo[j].objecto.pos.z -= 0.1;
			//if (modeloInimigo[j].objecto.pos.z > 2)	modeloInimigo[j].objecto.vida -= 0.5;
		}
		else modeloInimigo[j].objecto.pos.z = 1.5;
	}
	for (i = 0; i < numObstaculos; i++) {
		if (nx >= obstaculos[i].xi && nx <= obstaculos[i].xf && ny >= obstaculos[i].yi && ny <= obstaculos[i].yf || nx <= obstaculos[i].xi && nx >= obstaculos[i].xf && ny <= obstaculos[i].yi && ny >= obstaculos[i].yf) {
			return true;
		}
	}
	return false;
}

static void drawTorus(float r, float R, int nsides, int rings) {
	float ringDelta = 2.0f * (float)3.14 / rings;
	float sideDelta = 2.0f * (float)3.14 / nsides;
	float theta = 0.0f, cosTheta = 1.0f, sinTheta = 0.0f;
	for (int i = rings - 1; i >= 0; i--) {
		float theta1 = theta + ringDelta;
		float cosTheta1 = (float)cos(theta1);
		float sinTheta1 = (float)sin(theta1);
		glBegin(GL_QUAD_STRIP);
		float phi = 0.0f;
		for (int j = nsides; j >= 0; j--) {
			phi += sideDelta;
			float cosPhi = (float)cos(phi);
			float sinPhi = (float)sin(phi);
			float dist = R + r * cosPhi;
			glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
			glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
			glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
			glVertex3f(cosTheta * dist, -sinTheta * dist, r * sinPhi);
		}
		glEnd();
		theta = theta1;
		cosTheta = cosTheta1;
		sinTheta = sinTheta1;
	}
}

///////////////////////////////////////////////////////////////////////////////////Incio Metodos para explosao

void
newSpeed(float dest[3])
{
	float    x;
	float    y;
	float    z;
	float    len;

	x = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	y = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	z = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;

	/*
	* Normalizing the speed vectors gives a "fireball" effect
	*
	*/

	if (wantNormalize)
	{
		len = sqrt(x * x + y * y + z * z);

		if (len)
		{
			x = x / len;
			y = y / len;
			z = z / len;
		}
	}

	dest[0] = x;
	dest[1] = y;
	dest[2] = z;
}


void
newExplosion(Donut d)
{
	int    i;

	for (i = 0; i < NUM_PARTICLES; i++)
	{
		particles[i].position[0] = d.pos.x;
		particles[i].position[1] = d.pos.y;
		particles[i].position[2] = d.pos.z;

		particles[i].color[0] = 1.0;
		particles[i].color[1] = 1.0;
		particles[i].color[2] = 0.5;

		newSpeed(particles[i].speed);
	}

	fuel = 30;
	decaimento = 0;
}



void
displayExplosion(void)
{
	int    i;

	if (fuel > 0)
	{
		glPushMatrix();

		glBegin(GL_POINTS);

		for (i = 0; i < NUM_PARTICLES - decaimento; i++)
		{
			glColor3fv(particles[i].color);
			glVertex3fv(particles[i].position);
		}

		glEnd();

		glPopMatrix();

		glNormal3f(0.0, 0.0, 1.0);
	}
}


void redrawExplosion(void)
{
	int    i;

	if (!wantPause)
	{
		if (fuel > 0)
		{
			for (i = 0; i < NUM_PARTICLES - decaimento; i++)
			{
				decaimento++;
				particles[i].position[0] += particles[i].speed[0] * 0.6;
				particles[i].position[1] += particles[i].speed[1] * 0.6;
				particles[i].position[2] += particles[i].speed[2] * 0.6;

				particles[i].color[0] -= 1.0 / 500.0;
				if (particles[i].color[0] < 0.0)
				{
					particles[i].color[0] = 0.0;
				}

				particles[i].color[1] -= 1.0 / 100.0;
				if (particles[i].color[1] < 0.0)
				{
					particles[i].color[1] = 0.0;
				}

				particles[i].color[2] -= 1.0 / 50.0;
				if (particles[i].color[2] < 0.0)
				{
					particles[i].color[2] = 0.0;
				}
			}

			--fuel;

		}

		//angle += 0.3;  /* Always continue to rotate the camera */
	}

	displayExplosion();
}
/////////////////////////////////////////////////////////////////////////////////////////Fim metodos para explosao

void desenhaDonut(){
	tipo_material cores[10] = { brass, red_plastic, emerald, slate, azul, preto, cinza, brass, red_plastic, emerald };
	int i = 0;
	redrawExplosion();
	for (i = 0; i < 10; i++){
		bool condicao = colisao(modelo.objecto.donut[i].pos.x, modelo.objecto.donut[i].pos.y, "donut");

		if (condicao == false && modelo.objecto.donut[i].pos.x <= 200 && modelo.objecto.donut[i].pos.x >= -200 && modelo.objecto.donut[i].pos.y <= 200 && modelo.objecto.donut[i].pos.y >= -200){

			modelo.objecto.donut[i].pos.x = modelo.objecto.donut[i].pos.x + cos(modelo.objecto.donut[i].dir) * 0.9;
			modelo.objecto.donut[i].pos.y = modelo.objecto.donut[i].pos.y + sin(modelo.objecto.donut[i].dir) * 0.9;
			glPushMatrix();
			material(cores[i]);
			glTranslatef(modelo.objecto.donut[i].pos.x, modelo.objecto.donut[i].pos.y, modelo.objecto.donut[i].pos.z + 0.5);
			if (modelo.objecto.donut[i].ativo) drawTorus(0.1, 0.2, 30, 30);
			glPopMatrix();
		}
		else if (modelo.objecto.donut[i].ativo)
		{
			modelo.objecto.donut[i].ativo = false;
			newExplosion(modelo.objecto.donut[i]);
			//Ativa som de explosao
			InitAudio("explosao");
			alGetSourcei(estado.sourceExplosao, AL_SOURCE_STATE, &stateExplosao);
			alSourcePlay(estado.sourceExplosao);
		}
	}
}

void desenhaLanterna()
{
	glPushMatrix();
	if (modelo.andar)
	{
		glTranslatef(0.3, 0, 0);
	}
	glTranslatef(0, 0, 2.1);
	glutSolidTorus(0.1, 0.5, 15, 15);
	glTranslatef(0.67, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	glutSolidCone(0.2, 0.1, 15, 15);

	const GLfloat spot_dir[] = { 0.0, 0.0, -1.0 };
	GLfloat pos0[] = { 0, 0, 0.1, 1 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.0, 0.2 };
	GLfloat light_diffuse[] = { 0.3, 0.3, 0.0, 0.3 };
	GLfloat light_specular[] = { 0.3, 0.3, 0.0, 0.3 };


	glLightfv(GL_LIGHT7, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT7, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT7, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT7, GL_POSITION, pos0);
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, spot_dir);
	glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 20.0f);
	glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.02);

	glPopMatrix();
}

int cont_pos = -1;
//desenha o homer no centro do mapa 
void desenhaHomer(){
	/*cont_pos++;
	if(cont_pos<MAX_POS) modelo.objecto.posicoes[cont_pos] = modelo.objecto.pos;*/


	//else cout << "sem posicoes de memoria para gravar percurso" << endl;
	glPushMatrix();

	glPopMatrix();
	glPushMatrix();

	glTranslatef(modelo.objecto.pos.x, modelo.objecto.pos.y, modelo.objecto.pos.z);

	glRotatef(graus(modelo.objecto.dir), 0, 0, 1);

	desenhaLanterna();
	//glRotatef(0, 1, 0, 0);
	glScalef(SCALE_HOMER, SCALE_HOMER, SCALE_HOMER);
	mdlviewer_display(modelo.homer);

	glPopMatrix();

}


void desenhaInimigo(string same){
	if (same == "same"){
		for (int i = 0; i < quantidadeInimigosVisiveis; i++)
		{

			if (modeloInimigo[i].visivel) {
				//tipo de inimigo 1 (outros tipos vao ser feitos alterando a cor do material)
				glPushMatrix();
				material(emerald);
				glTranslatef(modeloInimigo[i].objecto.pos.x, modeloInimigo[i].objecto.pos.y, modeloInimigo[i].objecto.pos.z + OBJECTO_ALTURA);
				glRotatef(graus(modeloInimigo[i].objecto.dir), 0, 0, 1);
				glRotatef(0, 1, 0, 0);
				glScalef(SCALE_HOMER, SCALE_HOMER, SCALE_HOMER);
				mdlviewer_display(modeloInimigo[i].inimigo);
				glPopMatrix();
				desenhaBillBoard(modeloInimigo[i]);
			}
		}
	}
	else{
		for (int i = 0; i < quantidadeInimigosVisiveis; i++)
		{

			if (modeloInimigo[i].visivel) {
				//tipo de inimigo 1 (outros tipos vao ser feitos alterando a cor do material)
				glPushMatrix();
				//material(emerald);
				glTranslatef(modeloInimigo[i].objecto.pos.x, modeloInimigo[i].objecto.pos.y, modeloInimigo[i].objecto.pos.z + OBJECTO_ALTURA);
				glRotatef(graus(modeloInimigo[i].objecto.dir), 0, 0, 1);
				glRotatef(0, 1, 0, 0);
				glScalef(SCALE_HOMER, SCALE_HOMER, SCALE_HOMER);
				mdlviewer_display(modeloInimigo[i].inimigo);
				glPopMatrix();
				desenhaBillBoard(modeloInimigo[i]);
			}
		}

	}
}

int cor = 0;
int contagemAnimacao = 0;
void desenhaAnimacaoQuartoSecreto(){
	if (flagAnimacao){

		modelo.objecto.dir++;

		if (contagemAnimacao <= 50){
			modelo.objecto.pos.z++;
		}
		else{ modelo.objecto.pos.z--; }
		/*cor++;
		if (cor == 4) cor = 0;
		glPushMatrix();
		const float cores[3][3] = { { 1.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 } };

		glBegin(GL_QUADS);
		glColor3fv(cores[cor]);
		glVertex2f(0, 0);
		glVertex2f(650, 0);
		glVertex2f(650, 500);
		glVertex2f(0, 500);
		glEnd();


		glPopMatrix();*/
	}
}

void escreverTexto(int x, int y, float r, float g, float b, char *string)
{
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

void desenhaBotao(float xini, float yini, float xfim, float yfim, float cor[3])
{
	glColor3fv(cor);
	glBegin(GL_QUADS);
	glVertex2f(xini, yini);
	glVertex2f(xini, yfim);
	glVertex2f(xfim, yfim);
	glVertex2f(xfim, yini);
	glEnd();
}

void desenhaGameOver(){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_INIT_WINDOW_WIDTH), 0, glutGet(GLUT_INIT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);

	float cinzento[3] = { 0.3, 0.3, 0.3 };
	string label = "";

	label = "Game Over";
	char* p = new char[label.length() + 1];
	memcpy(p, label.c_str(), label.length() + 1);
	escreverTexto(270, 300, 500, 400, 1, p);

	desenhaBotao(270, 70, 330, 150, cinzento);
	label = "OK";
	p = new char[label.length() + 1];
	memcpy(p, label.c_str(), label.length() + 1);
	escreverTexto(290, 105, 1, 1, 1, p);

	glEnable(GL_LIGHTING);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void desenhaVidaHomer(){
	glPushMatrix();
	const int numDiv = 3;
	const float sep = 2;
	const float barHeight = 20.0 / (float)numDiv;
	const float cores[3][3] = { { 1.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 } };
	float altura = 0;
	int z = modelo.objecto.vida / 35;
	int resto = modelo.objecto.vida - (z * 35);
	
	if (modelo.vida <= 0){
		//homer morreu
		flagJogadorMorto = true;
		desenhaGameOver();
	}

	for (int i = 0; i < modelo.vida; i++){
		glBegin(GL_QUADS);
		glColor3fv(cores[i]);
		if (i * 35 <= modelo.objecto.vida && (modelo.objecto.vida >= (i - 1) * 35)){
			glVertex2f(600, altura + 30);
			glVertex2f(650, altura + 30);
			glVertex2f(650, altura + barHeight + 30);
			glVertex2f(600, altura + barHeight + 30);
		}
		else{
			if (modelo.objecto.vida < 0){
				//cout << "morreu" << endl;
			}
		}
		glEnd();
		altura += barHeight + sep;
	}
	glPopMatrix();
}

string mundoACarregar;
string quartoACarregar;

void carregaMundo()
{
	menu.menuActivo = GL_FALSE;

	for (int i = 0; i < menu.totalMundos; i++)
	{
		if (menu.mundos[i] == menu.mundoSelecionado)
		{
			mundoACarregar = menu.fichMundos[i];
			mundoAtual = i;
			//quartoACarregar = menu.quartoDeMundo[i][quartoAtual];
		}
	}
	leGrafoMundo(callService_DownloadMapa(mundoACarregar));
	fimMapa.pontuacao = pontuacao;
}

void carregaNivel(string mundo)
{

	//desenhaLoadScreen();
	menu.menuActivo = GL_FALSE;
	//loadActivo = GL_TRUE;
	//glutPostRedisplay();

	for (int i = 0; i < menu.totalMundos; i++)
	{
		if (menu.mundos[i] == menu.mundoSelecionado)
		{
			//mundoACarregar = menu.fichMundos[i];
			quartoACarregar = menu.quartoDeMundo[i][quartoAtual];
		}
	}

	//if (menu.mundos[0] == menu.mundoSelecionado)
	//{
	//	mundoACarregar = menu.fichMundos[0];
	//	quartoACarregar = menu.quartoDeMundo[0][0];
	//}
	//else
	//{
	//	mundoACarregar = menu.fichMundos[1];
	//	quartoACarregar = menu.quartoDeMundo[1][quartoAtual];//[contaQuarto];
	//}


	/*setFicheiroMundo(callService_DownloadMapa(mundoACarregar));
	setFicheiroQuarto(callService_DownloadMapa(quartoACarregar));*/
	flag = 0;
	//leGrafoMundo(callService_DownloadMapa(mundoACarregar));
	leGrafoQuarto(callService_DownloadMapa(quartoACarregar));
	//inicia particulas
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		initParticulas(i);
	}
	jogoActivo = GL_TRUE;


	initModelo();
	initModeloInimigo();
	initEstado();
	modelo.quad = gluNewQuadric();
	gluQuadricDrawStyle(modelo.quad, GLU_FILL);
	gluQuadricNormals(modelo.quad, GLU_OUTSIDE);

	for (int i = 0; i < quantidadeInimigosVisiveis; i++)
	{
		modeloInimigo[i].quad = gluNewQuadric();
		gluQuadricDrawStyle(modeloInimigo[i].quad, GLU_FILL);
		gluQuadricNormals(modeloInimigo[i].quad, GLU_OUTSIDE);
		gluQuadricDrawStyle(modeloInimigo[i].quad, GLU_FILL);
		gluQuadricNormals(modeloInimigo[i].quad, GLU_OUTSIDE);
	}

	for (size_t i = 0; i < quantidadeInimigosVisiveis; i++)
	{
		mdlviewer_init("homer.mdl", modeloInimigo[i].inimigo);
	}
}







void desenhaLogin()
{

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_INIT_WINDOW_WIDTH), 0, glutGet(GLUT_INIT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);









	float branco[3] = { 1.0, 1.0, 1.0 };
	float cinzento[3] = { 0.3, 0.3, 0.3 };
	string nome = "Utilizador:";

	char* p = new char[nome.length() + 1];
	memcpy(p, nome.c_str(), nome.length() + 1);

	escreverTexto(50, 300, 1, 1, 1, p);


	p = new char[login.nomeUtilizador.length() + 1];
	memcpy(p, login.nomeUtilizador.c_str(), login.nomeUtilizador.length() + 1);

	escreverTexto(155, 305, 0, 0, 0, p);
	desenhaBotao(150, 300, 550, 350, branco);

	nome = "Password:";

	p = new char[nome.length() + 1];
	memcpy(p, nome.c_str(), nome.length() + 1);

	escreverTexto(50, 200, 1, 1, 1, p);

	p = new char[login.password.length() + 1];
	memcpy(p, login.password.c_str(), login.password.length() + 1);

	escreverTexto(155, 205, 0, 0, 0, p);
	desenhaBotao(150, 200, 550, 250, branco);

	nome = "Login";

	p = new char[nome.length() + 1];
	memcpy(p, nome.c_str(), nome.length() + 1);

	escreverTexto(120, 105, 1, 1, 1, p);
	desenhaBotao(100, 90, 200, 130, cinzento);


	glEnable(GL_LIGHTING);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}


void desenhaFimMundo()
{

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_INIT_WINDOW_WIDTH), 0, glutGet(GLUT_INIT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);

	float branco[3] = { 1.0, 1.0, 1.0 };
	float cinzento[3] = { 0.3, 0.3, 0.3 };

	if (menu.mundoSelecionado[0]){
		string nome = "Pontuacao: " + std::to_string(fimMapa.pontuacao);

		char * p = new char[nome.length() + 1];
		memcpy(p, nome.c_str(), nome.length() + 1);

		escreverTexto(230, 300, 1, 1, 1, p);

		desenhaBotao(50, 50, 220, 100, cinzento);
		escreverTexto(70, 70, 1, 1, 1, "Repetir Mundo");

		desenhaBotao(220, 170, 400, 220, cinzento);
		escreverTexto(230, 190, 1, 1, 1, "Upload Pontuacao");

		desenhaBotao(400, 50, 590, 100, cinzento);
		escreverTexto(420, 70, 1, 1, 1, "Mundo Seguinte");

		/*desenhaBotao(glutGet(GLUT_WINDOW_WIDTH) - 150, 220, glutGet(GLUT_WINDOW_WIDTH) - 50, 300, cinzento);
		escreverTexto(glutGet(GLUT_WINDOW_WIDTH) - 150, 270, 1, 1, 1, "Mundo Seguinte");*/

	}

	else{
		string nome = "Pontuacao: " + std::to_string(fimMapa.pontuacao);

		char * p = new char[nome.length() + 1];
		memcpy(p, nome.c_str(), nome.length() + 1);





		escreverTexto(230, 300, 1, 1, 1, p);

		desenhaBotao(50, 50, 220, 100, cinzento);
		escreverTexto(70, 70, 1, 1, 1, "Repetir Mundo");
	}


	glEnable(GL_LIGHTING);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void desenhaMenu()
{

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_INIT_WINDOW_WIDTH), 0, glutGet(GLUT_INIT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);

	float branco[3] = { 1.0, 1.0, 1.0 };
	float cinzento[3] = { 0.3, 0.3, 0.3 };
	escreverTexto(300, 95, 0, 0, 0, "Jogar");
	desenhaBotao(50, 50, 590, 150, cinzento);


	escreverTexto(70, 270, 0, 0, 0, "Anterior");
	desenhaBotao(50, 250, 150, 300, cinzento);

	escreverTexto(500, 270, 0, 0, 0, "Seguinte");
	desenhaBotao(490, 250, 590, 300, cinzento);

	char* p = new char[menu.mundoSelecionado.length() + 1];
	memcpy(p, menu.mundoSelecionado.c_str(), menu.mundoSelecionado.length() + 1);

	escreverTexto(280, 270, 0, 0, 0, p);
	desenhaBotao(200, 250, 440, 300, branco);


	glEnable(GL_LIGHTING);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}

void desenhaHUD(void)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	/*glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);*/
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_INIT_WINDOW_WIDTH), 0, glutGet(GLUT_INIT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	setCamera();*/
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);

	/*glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex2f(50, 50);
	glVertex2f(250, 50);
	glVertex2f(250, 250);
	glVertex2f(50, 250);
	glEnd();*/

	desenhaAnimacaoQuartoSecreto();
	desenhaVidaHomer();

	/*glMatrixMode(GL_PROJECTION);*/
	/*glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);*/

	glEnable(GL_LIGHTING);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void subWindowInit(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

void ReshapeMinimap(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLdouble)width / height, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display(void)
{
	if (login.loginActivo)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		desenhaLogin();
	}

	if (menu.menuActivo)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		desenhaMenu();
	}

	if (jogoActivo)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		setCamera();

		material(slate);
		desenhaSolo(modelo.texID[ID_TEXTURA_CHAO]);


		//desenhaEixos();



		desenhaLabirinto(numNosMundo, nosMundo, numArcosMundo, arcosMundo, 0, NULL, modelo, estado);
		desenhaLabirinto(numNosQuarto, nosQuarto, numArcosQuarto, arcosQuarto, numRampasQuarto, rampas, modelo, estado);

		/*if (estado.eixoTranslaccao) {
			cout << "Translate... " << estado.eixoTranslaccao << endl;
			desenhaPlanoDrag(estado.eixoTranslaccao);

			}*/

		desenhaHomer();
		desenhaDonut();

		desenhaObstaculos(menu, quantidadeObstaculos, posXObstaculos, posYObstaculos);

		desenhaInimigo(sameMDL);
		//desenha particulas
		if (estado.tipoParticulas == CHUVA && estado.teclas.p)
		{
			desenhaChuva();
		}
		if (estado.tipoParticulas == NEVE && estado.teclas.p)
		{
			desenhaNeve();
		}
		if (estado.tipoParticulas == GRANIZO && estado.teclas.p)
		{
			desenhaGranizo();
		}
		if (estado.tipoParticulas == CLEAR && estado.teclas.p)
		{
			retiraSom();
		}

		if (chegouFimMundo){
			fimMapa.FimMundoActivo = GL_TRUE;
			jogoActivo = GL_FALSE;
			chegouFimMundo = false;
			fimMundo.mundoAnterior = mundoAtual;
			mundoAtual++;
			fimMundo.mundoSeguinte = mundoAtual;
		}
		else{
			if (chegouFim){
				if (menu.mundoSelecionado == menu.mundos[0]){
					fimMapa.FimMundoActivo = GL_TRUE;
					jogoActivo = GL_FALSE;
					chegouFim = false;
				}
				else{
					fimMapa.FimQuarto = GL_TRUE;
					chegouFim = false;
				}
			}
			else{
				desenhaHUD();
			}
		}
	}

	if (fimMapa.FimMundoActivo)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		aTimer.stop();
		long res = aTimer.getTime();
		fimMapa.pontuacao = fimMapa.pontuacao - res;
		aTimer.reset();

		desenhaFimMundo();
	}

	glFlush();
	glutSwapBuffers();

}

void displayMinimapa(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	TopCamLookAt();
	material(slate);

	if (jogoActivo)
	{
		desenhaSolo(modelo.texID[0]);
	}

	desenhaLabirinto(numNosMundo, nosMundo, numArcosMundo, arcosMundo, 0, NULL, modelo, estado);
	desenhaLabirinto(numNosQuarto, nosQuarto, numArcosQuarto, arcosQuarto, numRampasQuarto, rampas, modelo, estado);

	desenhaHomer();
	desenhaDonut();

	desenhaObstaculos(menu, quantidadeObstaculos, posXObstaculos, posYObstaculos);

	desenhaInimigo(sameMDL);

	glFlush();
	glutSwapBuffers();

}

void DesenhaMinimapa(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//draw subscene
	displayMinimapa();
	redisplayAll();
	glFlush();
}


float coeficiente(float x, float y, float x2, float y2){
	return (y2 - y) / (x2 - x);
}

bool colisaoFim(GLfloat nx, GLfloat ny, string tipo)
{
	if (detetaColisoes == false) return false;
	if (modelo.objecto.pos.y >= (ny - 3) && modelo.objecto.pos.y <= (ny + 3) && modelo.objecto.pos.x >= (nx - 3) && modelo.objecto.pos.x <= (nx + 3)){
		if (tipo != "fimMundo") flagAnimacao = true;
		return true;
	}
	else{
		return false;
	}
}

bool gravaPercurso(){
	if (callService_InsertTrack(login.nomeUtilizador, quartoACarregar, modelo.objecto.posicoes)){
		cout << "percurso gravado" << endl;
		return true;
	}
	else cout << "não foi possivel gravar percurso" << endl;
	return false;
}

bool colisaoPersonagemInimigo(GLfloat nx, GLfloat ny, GLfloat nxInimigo, GLfloat nyInimigo) {
	//Condicao a satisfazer: (raioHomer+raioInimigo)2 <= (nxInimigo-nx)2+(nyInimigo-ny)2
	if (pow((0.8 + 0.8), 2) >= pow((nxInimigo - nx), 2) + pow((nyInimigo - ny), 2)) //0.8 e o raio estimado do homer e do inimigo
		return true;
	else
		return false;
}

//movimentos do homer 
void MovimentacaoPersonagem(){
	if (flagAnimacao) return;
	GLfloat xAntigo, yAntigo;
	xAntigo = modelo.objecto.pos.x;
	yAntigo = modelo.objecto.pos.y;

	GLfloat nx, ny;
	modelo.andar = GL_FALSE;


	GLuint curr = GetTickCount();
	aTimer.start();
	float velocidade = modelo.objecto.vel;//*float velocidade = modelo.objecto.vel*(curr - modelo.prev)*0.01;
	modelo.prev = curr;



	for (size_t i = 0; i < quantidadeAlcapoes; i++)
	{
		if ((float)modelo.objecto.pos.x >= posxalcapao[i] * 5 - 2.4 && (float)modelo.objecto.pos.x <= posxalcapao[i] * 5 + 2.4 && (float)modelo.objecto.pos.y >= posyalcapao[i] * 5 - 2.4 && (float)modelo.objecto.pos.y <= posyalcapao[i] * 5 + 2.4){
			inAlcapao = true;
		}
	}
	if (inAlcapao && modelo.objecto.pos.z>-5){

		modelo.objecto.pos.z -= 0.5;
		modelo.vida = 0;
		glutPostRedisplay();
	}
	inAlcapao = false;




	if (estado.teclas.up){
		modelo.objecto.posicoes += to_string(modelo.objecto.pos.x) + "," + to_string(modelo.objecto.pos.y) + "," + to_string(modelo.objecto.pos.z) + ";";
		nx = modelo.objecto.pos.x + cos(modelo.objecto.dir)*velocidade;


		ny = modelo.objecto.pos.y + sin(modelo.objecto.dir)*velocidade;







		if (!colisao(nx, ny, "personagem")){

			modelo.objecto.pos.x = nx;
			modelo.objecto.pos.y = ny;
			modelo.andar = GL_TRUE;

		}


	}
	if (estado.teclas.down){
		modelo.objecto.posicoes += to_string(modelo.objecto.pos.x) + "," + to_string(modelo.objecto.pos.y) + "," + to_string(modelo.objecto.pos.z) + ";";
		nx = modelo.objecto.pos.x - cos(modelo.objecto.dir)*velocidade;
		ny = modelo.objecto.pos.y - sin(modelo.objecto.dir)*velocidade;

		/*if (menu.mundoSelecionado == menu.mundos[0])
		{
		colisaoFim(20, 35);
		}*/

		if (!colisao(nx, ny, "personagem")){
			modelo.objecto.pos.x = nx;
			modelo.objecto.pos.y = ny;
			modelo.andar = GL_TRUE;
		}



	}
	if (estado.teclas.left){
		modelo.objecto.dir += rad(OBJECTO_ROTACAO);
		//if (vista_atual != "vista_superior"){//camera 3 pessoa e 1 
		estado.camera.dir_long += rad(OBJECTO_ROTACAO);


		//}
	}
	if (estado.teclas.right){
		modelo.objecto.dir -= rad(OBJECTO_ROTACAO);
		//if (vista_atual != "vista_superior"){ //camera 3 pessoa e 1 
		estado.camera.dir_long -= rad(OBJECTO_ROTACAO);
		//}
	}
	if (modelo.homer.GetSequence() != 20)
	{
		if (modelo.andar && modelo.homer.GetSequence() != 3)
		{
			modelo.homer.SetSequence(3);
		}
		else if (!modelo.andar && modelo.homer.GetSequence() != 0)
		{
			modelo.homer.SetSequence(0);
		}
	}

	for (int i = 0; i < quantidadeInimigosVisiveis; i++) {
		if (colisaoPersonagemInimigo(modelo.objecto.pos.x, modelo.objecto.pos.y, modeloInimigo[i].objecto.pos.x, modeloInimigo[i].objecto.pos.y)  && modeloInimigo[i].visivel) {
			modelo.objecto.pos.x = xAntigo;
			modelo.objecto.pos.y = yAntigo;
		}
	}
	chegouFimMundo = colisaoFim(posxfinal[0] * 5, posyfinal[0] * 5, "fimMundo");

	for (int i = 0; i < numPosicaoTrans; i++){
		if (colisaoFim(posxtrans[i] * 5, posytrans[i] * 5, "fimQuarto")){
			fimMapa.FimQuarto = GL_TRUE;
			fimQuarto.FimQuartoActivo = GL_TRUE;
			fimQuarto.QuartoAnterior = quartoAtual;
			fimQuarto.QuartoSeguinte = proxquarto[i];
			fimQuarto.posinicio = posxfinais[i];
			fimQuarto.posfim = posyfinais[i];
			fimQuarto.i = i;
			quartoAtual = proxquarto[i] - 1;

			gravaPercurso();
		}

	}
	glutPostRedisplay();
}


boolean DetetaHomer(float x, float y, float z){
	//distancia entre o inimigo e o homer.
	float distanciaXdoInimigoAteHomer = x - modelo.objecto.pos.x;
	float distanciaYdoInimigoAteHomer = y - modelo.objecto.pos.y;
	float distanciaZdoInimigoAteHomer = z - modelo.objecto.pos.z;



	//o sqrt é para passar a distancia para positivo sempre.
	if (abs(distanciaXdoInimigoAteHomer) <= RangeParaInimigoDetetarHomer && abs(distanciaYdoInimigoAteHomer) <= RangeParaInimigoDetetarHomer && abs(distanciaZdoInimigoAteHomer) <= RangeParaInimigoDetetarHomer){
		return true;
	}

}













int j = 1;
boolean frente;
void MovimentacaoInimigos(){
	j++;
	for (size_t i = 0; i < quantidadeInimigosVisiveis; i++)
	{
		if (modeloInimigo[i].visivel){
		modeloInimigo[i].andar = GL_FALSE;
		GLfloat xAntigo, yAntigo;
		xAntigo = modeloInimigo[i].objecto.pos.x;
		yAntigo = modeloInimigo[i].objecto.pos.y;

		GLfloat nx, ny;

		//		GLuint curr = GetTickCount();
		float velocidade = modeloInimigo[i].objecto.vel;//**(curr - modeloInimigo[i].prev)*0.01;
		//		modeloInimigo[i].prev = curr;

		verificaSeHomerEstaNoRange[i] = DetetaHomer(modeloInimigo[i].objecto.pos.x, modeloInimigo[i].objecto.pos.y, modeloInimigo[i].objecto.pos.z);

		if (verificaSeHomerEstaNoRange[i]){

			if ((modelo.objecto.pos.x > modeloInimigo[i].objecto.pos.x)){
				nx = modeloInimigo[i].objecto.pos.x + cos(modeloInimigo[i].objecto.dir)*velocidade;
			}
			else if ((modelo.objecto.pos.x < modeloInimigo[i].objecto.pos.x)){

				nx = modeloInimigo[i].objecto.pos.x - cos(modeloInimigo[i].objecto.dir)*velocidade;

			}
			if ((modelo.objecto.pos.y > modeloInimigo[i].objecto.pos.y)){

				ny = modeloInimigo[i].objecto.pos.y + cos(modeloInimigo[i].objecto.dir)*velocidade;
			}
			else if ((modelo.objecto.pos.y < modeloInimigo[i].objecto.pos.y)){
				ny = modeloInimigo[i].objecto.pos.y - cos(modeloInimigo[i].objecto.dir)*velocidade;
			}
			if (!colisaoInimigo(nx, ny, "personagem", i)){
				modeloInimigo[i].andar = GL_TRUE;
				modeloInimigo[i].objecto.pos.x = nx;
				modeloInimigo[i].objecto.pos.y = ny;
			}
		}

		if (modeloInimigo[i].inimigo.GetSequence() != 20)
		{
			if (modeloInimigo[i].andar && modeloInimigo[i].inimigo.GetSequence() != 3)
			{

				modeloInimigo[i].inimigo.SetSequence(3);

			}
			else if (!modeloInimigo[i].andar && modeloInimigo[i].inimigo.GetSequence() != 0)
			{
				modeloInimigo[i].inimigo.SetSequence(0);
			}
		}

		if (colisaoPersonagemInimigo(modelo.objecto.pos.x, modelo.objecto.pos.y, modeloInimigo[i].objecto.pos.x, modeloInimigo[i].objecto.pos.y)  && modeloInimigo[i].visivel) {
			modeloInimigo[i].objecto.pos.x = xAntigo;
			modeloInimigo[i].objecto.pos.y = yAntigo;
		}

		verificaSeHomerEstaNoRange[i] = false;
		}
		
		//Caso o inimigo esteja a colidir com o personagem,retira-lhe vida
		float centro_x = modeloInimigo[i].objecto.pos.x;
		float centro_y = modeloInimigo[i].objecto.pos.y;
		float raio = 1.9; //raio aproximado do homer

		if ((pow((modelo.objecto.pos.x - centro_x), 2) + pow((modelo.objecto.pos.y - centro_y), 2)) <= pow(raio, 2) && modeloInimigo[i].visivel) {
			if (podeSerAtacado > 0) {
				modelo.vida -= 1;
				podeSerAtacado = -40;
			}
		}
	}
	glutPostRedisplay();
}

bool insereTextoUtilizador = false;

bool insereTextoPassword = false;

bool detetaAlcapaoQuartoSecreto() {
	for (int i = 0; i < quantidadeAlcapoesQuartoSecreto; i++) {
		if ((pow((modelo.objecto.pos.x - posxalcapaoQuartoSecreto[i] * 5), 2) + pow((modelo.objecto.pos.y - posyalcapaoQuartoSecreto[i] * 5), 2)) <= pow(4.0, 2))
			return true;
	}
	return false;
}
void MinimapToggle(bool active);
void Timer(int value)
{
	bool fimMundoAtivo = false;
	if (flagAnimacao == true && contagemAnimacao < 100){
		contagemAnimacao++;
	}
	else{
		if (flagAnimacao) estado.camera.dir_long = modelo.objecto.dir;
		contagemAnimacao = 0;
		flagAnimacao = false;
	}

	if (fimMapa.FimMundoActivo)
	{
		if (estado.xMouse > 50 * resolucaoX && estado.xMouse < 100 * resolucaoX && estado.yMouse > 50 * resolucaoY && estado.yMouse < 220 * resolucaoY && estado.mouseState == 0){
			fimMapa.FimMundoActivo = GL_FALSE;
			jogoActivo = GL_TRUE;
			//myInit();
		}

		if (estado.xMouse > 220 * resolucaoX && estado.xMouse < 400 * resolucaoX && estado.yMouse > 170 * resolucaoY && estado.yMouse < 220 * resolucaoY && estado.mouseState == 0){
			//upload pontuacao
			string map;
			bool prova = callService_UploadPontuacao(login.nomeUtilizador, fimMapa.pontuacao, menu.mundoSelecionado);
		}

		if (estado.xMouse > 400 * resolucaoX && estado.xMouse < 590 * resolucaoX && estado.yMouse > 50 * resolucaoY && estado.yMouse < 100 * resolucaoY && estado.mouseState == 0){
			if (fimMundo.mundoSeguinte < menu.totalMundos){
				menu.mundoSelecionado = menu.mundos[fimMundo.mundoSeguinte];
				carregaMundo();
				carregaNivel(menu.mundoSelecionado);
				numParedes = 0;
				fimMapa.FimMundoActivo = GL_FALSE;
				jogoActivo = GL_TRUE;
				fimMundo.mundoAnterior = fimMundo.mundoSeguinte;
			}
			else{
				fimMundoAtivo = true;
				fimMapa.FimMundoActivo = GL_FALSE;
				menu.menuActivo = GL_TRUE;
				jogoActivo = GL_FALSE;
				quartoAtual = 0;
				mundoAtual = 0;
				fimMundo.mundoSeguinte = 0;
				menu.mundoSelecionado = menu.mundos[fimMundo.mundoSeguinte];

			}



			//myInit();
		}
	}

	if (fimMapa.FimQuarto){
		menu.mundoSelecionado = menu.mundos[1];
		menu.quartoDeMundo[1][quartoAtual];
		carregaNivel(menu.mundoSelecionado);
		fimMapa.FimQuarto = GL_FALSE;
		myInit();
	}

	glutTimerFunc(estado.delay, Timer, 0);

	if (menu.menuActivo)
	{






		if (estado.xMouse > 50 * resolucaoX && estado.xMouse < 590 * resolucaoX && estado.yMouse > 50 * resolucaoY && estado.yMouse < 150 * resolucaoY && estado.mouseState == 0)
		{
			//if (!fimMundoAtivo){
			carregaMundo();
			carregaNivel(menu.mundoSelecionado);
			estado.xMouse = 0;
			estado.yMouse = 0;
			//}




		}


		if (estado.xMouse > 50 * resolucaoX && estado.xMouse < 150 * resolucaoX && estado.yMouse > 250 * resolucaoY && estado.yMouse < 300 * resolucaoY && estado.mouseState == 0)


		{







			cout << "Anterior " << estado.mouseState << endl;
			if (mundoAtual > 0)
			{
				mundoAtual--;
				menu.mundoSelecionado = menu.mundos[mundoAtual];
				desenhaMenu();
			}
		}


		if (estado.xMouse > 490 * resolucaoX && estado.xMouse < 590 * resolucaoX && estado.yMouse > 250 * resolucaoY && estado.yMouse < 300 * resolucaoY && estado.mouseState == 0)

		{
			cout << "Seguinte" << endl;
			if (mundoAtual < menu.totalMundos - 1)
			{
				mundoAtual++;
				menu.mundoSelecionado = menu.mundos[mundoAtual];
				desenhaMenu();
			}
		}




	}




	if (login.loginActivo)
	{
		if (estado.xMouse > 150 * resolucaoX && estado.xMouse < 550 * resolucaoX && estado.yMouse > 300 * resolucaoY && estado.yMouse < 350 * resolucaoY && estado.mouseState == 0)
		{
			if (insereTextoPassword) insereTextoPassword = false;
			insereTextoUtilizador = true;
			login.nomeUtilizador = "";

			//cin >> login.nomeUtilizador;
			desenhaLogin();
		}

		if (estado.xMouse > 150 * resolucaoX && estado.xMouse < 550 * resolucaoX && estado.yMouse > 200 * resolucaoY && estado.yMouse < 250 * resolucaoY && estado.mouseState == 0)
		{
			if (insereTextoUtilizador) insereTextoUtilizador = false;
			insereTextoPassword = true;
			login.password = "";

			//cin >> login.password;
			desenhaLogin();
		}

		if (estado.xMouse > 100 * resolucaoX && estado.xMouse < 200 * resolucaoX && estado.yMouse > 90 * resolucaoY && estado.yMouse < 130 * resolucaoY && estado.mouseState == 0)
		{
			cout << callService_CheckLogin(login.nomeUtilizador, login.password) << endl;
			if (callService_CheckLogin(login.nomeUtilizador, login.password)){
				login.loginActivo = false;
				menu.menuActivo = true;
			}

		}
	}

	if (jogoActivo)
	{


		ALint state;
		alGetSourcei(estado.source, AL_SOURCE_STATE, &state);
		float xAntigo = round(modelo.objecto.pos.x * 100) / 100;
		float yAntigo = round(modelo.objecto.pos.y * 100) / 100;
		MovimentacaoPersonagem();
		MovimentacaoInimigos();
		float xNovo = round(modelo.objecto.pos.x * 100) / 100;
		float yNovo = round(modelo.objecto.pos.y * 100) / 100;
		if (xAntigo != xNovo || yAntigo != yNovo)
			flagBateuNaParede = false;


		if (estado.tecla_s)



		{
			if (state != AL_PLAYING)
				alSourcePlay(estado.source);
		}
		else
			if (state == AL_PLAYING)
				alSourceStop(estado.source);







		//som da chuva
		/*if (estado.tecla_c)
		{
		if (stateChuva != AL_PLAYING)
		{
		InitAudio("chuva");
		alSourcePlay(estado.sourceChuva);
		}
		}
		else
		if (stateChuva == AL_PLAYING)
		alSourceStop(estado.sourceChuva);*/

		//efeito dia/noite
		if (estado.teclas.n)
		{
			estado.teclas.n = !estado.teclas.n;
			estado.noite = !estado.noite;
		}

		if (!estado.noite){
			//luz global ativa
			glEnable(GL_LIGHT0);

			//luz lanterna apaga
			glDisable(GL_LIGHT7);
		}
		else{
			//luz global apaga
			glDisable(GL_LIGHT0);

			//luz lanterna ativa
			glEnable(GL_LIGHT7);
		}

		/*if (estado.tempoDia < 0)
		{
		estado.noite = GL_FALSE;
		}
		else
		{
		estado.noite = GL_TRUE;
		}

		estado.tempoDia--;
		if (estado.tempoDia < -5000)
		{
		estado.tempoDia = 5000;

		}*/

		//alteração particulas
		estado.controloDeMudancaParticula--;
		if (estado.controloDeMudancaParticula < 0){
			estado.tipoParticulas = rand() % 4;
			estado.controloDeMudancaParticula = 200; //mais rapido a mudar de estado de tempo

			//estado.controloDeMudancaParticula = 2000; //mais lento a mudar de estado de tempo
		}

		for (int i = 0; i < quantidadeAlcapoesQuartoSecreto; i++) {
			if (detetaAlcapaoQuartoSecreto()) {
				fimMapa.FimQuarto = GL_TRUE;
				fimQuarto.FimQuartoActivo = GL_TRUE;
				fimQuarto.QuartoAnterior = quartoAtual;
				fimQuarto.QuartoSeguinte = 4;
				fimQuarto.posinicio = posxfinaisAlcapao[i];
				fimQuarto.posfim = posyfinaisAlcapao[i];
				fimQuarto.i = i;
				quartoAtual = 3;

				gravaPercurso();
				cout << "quarto secreto" << endl;
			}
		}

		podeSerAtacado += 1;
		
		if (flagJogadorMorto) {
			if (estado.xMouse > 270 * resolucaoX && estado.xMouse < 340 * resolucaoX && estado.yMouse > 70 * resolucaoY && estado.yMouse < 150 * resolucaoY) {
				flagJogadorMorto = false;
				quartoAtual = 0;
				carregaNivel(menu.mundoSelecionado);
				estado.xMouse = 0;
				estado.yMouse = 0;
			}
		}

		for (int i = 0; i < quantidadeInimigosVisiveis; i++) {
			if (modeloInimigo[i].vida <= 0) {
				modeloInimigo[i].visivel = false;
			}
		}
	}
	else if (!menu.menuActivo)
	{
		MinimapToggle(false);
	}
}

void MinimapToggle(bool active){
	if (active)
	{
		Window.Top = glutCreateSubWindow(Window.Main, 25, 25, 150, 150);
		subWindowInit();
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glutReshapeFunc(ReshapeMinimap);
		glutDisplayFunc(DesenhaMinimapa);
	}
	else
	{
		if (Window.Top != NULL && !login.loginActivo)
		{
			glutDestroyWindow(Window.Top);
			Window.Top = NULL;
		}
	}
}


void alternamodeloinimigo(){
	int j = 0;
	if (mdlactualinimigo == "homer.mdl"){
		//carregar p mdl2 	
		for (j = 0; j < quantidadeInimigosVisiveis; j++)
		{
			downloadModeloinimigo("Scream");
			mdlviewer_init("modeloinimigo.mdl", modeloInimigo[j].inimigo);
		}
		if (mdlactualjugador == mdlactualinimigo){
			sameMDL = "same";
		}
		else{
			sameMDL = "notsame";
		}
		mdlactualinimigo = "Scream.mdl";
	}
	else if (mdlactualinimigo == "Scream.mdl"){
		for (j = 0; j < quantidadeInimigosVisiveis; j++)
		{
			downloadModeloinimigo("ronny");
			mdlviewer_init("modeloinimigo.mdl", modeloInimigo[j].inimigo);
		}
		if (mdlactualjugador == mdlactualinimigo){
			sameMDL = "same";
		}
		else{
			sameMDL = "notsame";
		}
		mdlactualinimigo = "ronny.mdl";
	}
	else if (mdlactualinimigo == "ronny.mdl"){
		for (j = 0; j < quantidadeInimigosVisiveis; j++)
		{
			downloadModeloinimigo("homer");
			mdlviewer_init("modeloinimigo.mdl", modeloInimigo[j].inimigo);
		}
		if (mdlactualjugador == mdlactualinimigo){
			sameMDL = "same";
		}

		else{
			sameMDL = "notsame";
		}
		mdlactualinimigo = "homer.mdl";
	}
}

/* TEXTURAS */
#ifdef _WIN32

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File = NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File = fopen(Filename, "r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}
#endif

void createTextures(GLuint texID[])
{
	char *image;
	int w, h, bpp;

#ifdef _WIN32
	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture
	memset(TextureImage, 0, sizeof(void *) * 1);           	// Set The Pointer To NULL
#endif

	glGenTextures(NUM_TEXTURAS, texID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

#ifdef _WIN32
	if (TextureImage[0] = LoadBMP(NOME_TEXTURA_CUBOS))
	{
		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texID[ID_TEXTURA_CUBOS]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}
#else
	if (read_JPEG_file(NOME_TEXTURA_CUBOS, &image, &w, &h, &bpp))
	{
		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texID[ID_TEXTURA_CUBOS]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
#endif
	else
	{
		printf("Textura %s not Found\n", NOME_TEXTURA_CUBOS);
		exit(0);
	}

	string str = nomeTexturaChao;
	char * writable = new char[str.size() + 1];
	copy(str.begin(), str.end(), writable);
	writable[str.size()] = '\0';
	if (read_JPEG_file(writable, &image, &w, &h, &bpp))
	{
		glBindTexture(GL_TEXTURE_2D, texID[ID_TEXTURA_CHAO]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	else{
		printf("Textura %s not Found\n", writable);
		exit(0);

	}
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void alteraTextura(){
	if (nomeTexturaChao == "Chao.jpg")
		nomeTexturaChao = "Chao2.jpg";
	else
		nomeTexturaChao = "Chao.jpg";
	createTextures(modelo.texID);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 8){
		if (insereTextoUtilizador){
			login.nomeUtilizador = login.nomeUtilizador.substr(0, login.nomeUtilizador.length() - 1);
		}
		if (insereTextoPassword) login.password = login.password.substr(0, login.password.length() - 1);

	}
	else{
		if (key != 13 && key != 9){
			if (insereTextoUtilizador) login.nomeUtilizador += key;
			if (insereTextoPassword) login.password += key;
		}
	}
	if (jogoActivo){
		switch (key)
		{
		case 27:
			gravaPercurso();
			exit(0);
			break;
		case 'h':
		case 'H':
			imprime_ajuda();
			break;
		case 's':
		case 'S':
			estado.tecla_s = !estado.tecla_s;
			break;
		case 't':
		case 'T':
			alSourceStop(estado.source);
			InitAudio("musica_fundo");
			break;
		case 'r':
		case 'R':
			alSourceStop(estado.source);
			InitAudio("musica_fundo2");
			break;
		case 'd':
		case 'D':
			downloadMusica();
			break;
		case 'y':
		case 'Y':
			alteraTextura();
			break;
		case 'u':
		case 'U':
			downloadTextura();
			break;
		case 'c':
		case 'C':
			estado.tecla_c = !estado.tecla_c;
			break;
		case 'm':
		case 'M':
			MinimapToggle(estado.teclas.m);
			estado.teclas.m = !estado.teclas.m;
			
			break;
		case 'n':
		case 'N':
			estado.teclas.n = !estado.teclas.n;
			break;
		case 'p':
		case 'P':
			estado.teclas.p = !estado.teclas.p;
			if (estadoDoTempo == "chuva")
			{
				ALint stateChuva;
				alGetSourcei(estado.sourceChuva, AL_SOURCE_STATE, &stateChuva);
				if (stateChuva == AL_PLAYING) {
					alSourceStop(estado.sourceChuva);
					estadoDoTempo = "null";
				}
			}
			else if (estadoDoTempo == "granizo")
			{
				ALint stateGranizo;
				alGetSourcei(estado.sourceGranizo, AL_SOURCE_STATE, &stateGranizo);
				if (stateGranizo == AL_PLAYING) {
					alSourceStop(estado.sourceGranizo);
					estadoDoTempo = "null";
				}
			}
			else if (estadoDoTempo == "neve")
			{
				ALint stateNeve;
				alGetSourcei(estado.sourceNeve, AL_SOURCE_STATE, &stateNeve);
				if (stateNeve == AL_PLAYING){
					alSourceStop(estado.sourceNeve);
					estadoDoTempo = "null";
				}
			}
			break;
		case 'Z':
		case 'z': if (detetaColisoes){
			cout << "colisoes desativadas" << endl;
			detetaColisoes = false;
		}
				  else{
					  cout << "colisoes ativadas" << endl;
					  detetaColisoes = true;
				  }
				  break;
		case ' ':

			for (int i = 0; i < 10; i++){
				if (modelo.objecto.donut[i].ativo == false){
					modelo.objecto.donut[i].ativo = true;
					modelo.objecto.donut[i].pos = modelo.objecto.pos;
					modelo.objecto.donut[i].dir = modelo.objecto.dir;
					//Ativa som do disparo
					alGetSourcei(estado.sourceDonut, AL_SOURCE_STATE, &stateDonut);
					if (stateDonut != AL_PLAYING) {
						InitAudio("disparoDonut");
						alSourcePlay(estado.sourceDonut);
					}
					break;
				}
			}
			break;


			//muda modelo do inimigo

		case ',':
			//int j=0;
			//if (mdlactualinimigo == "homer.mdl"){
			//	//carregar p mdl2 	
			//	for ( j = 0; j < quantidadeInimigosVisiveis; j++)
			//	{
			//		mdlviewer_init(nomeMDLinimigo2, modeloInimigo[j].inimigo);
			//	}
			//	if (mdlactualjugador == mdlactualinimigo){
			//		sameMDL = "same";
			//	}
			//	else{
			//		sameMDL = "notsame";
			//	}
			//}
			//else if (mdlactualinimigo == "Sream.mdl"){
			//	for ( j = 0; j < quantidadeInimigosVisiveis; j++)
			//	{
			//		mdlviewer_init(nomeMDLinimigo3, modeloInimigo[j].inimigo);
			//	}
			//	if (mdlactualjugador == mdlactualinimigo){
			//		sameMDL = "same";
			//	}
			//	else{
			//		sameMDL = "notsame";
			//	}
			//}
			//else if (mdlactualinimigo == "ronny.mdl"){
			//	for ( j = 0; j < quantidadeInimigosVisiveis; j++)
			//	{
			//		mdlviewer_init(nomeMDLinimigo1, modeloInimigo[j].inimigo);
			//	}
			//	if (mdlactualjugador == mdlactualinimigo){
			//		sameMDL = "same";
			//	}
			//	else{
			//		sameMDL = "notsame";
			//	}
			//}
			alternamodeloinimigo();
			break;
		}

	}
}

void alteraVista()
{
	if (vista_atual == "vista_superior_inclinada"){ vista_atual = "vista_superior"; }

	if (vista_atual == "terceira_pessoa")
	{
		/* Primeira Pessoa */
		//estado.camera.dir_long = rad(180);
		//estado.camera.dir_lat = rad(180);
		estado.camera.fov = 60;
		vista_atual = "primeira_pessoa";
		printf("Vista atual: primeira_pessoa\n");
		glutPostRedisplay();
	}
	else if (vista_atual == "primeira_pessoa")
	{
		/* Vista Superior */
		/*double lim = M_PI / 2 - 0.1;
		longitude = estado.camera.dir_long;
		latitude = estado.camera.dir_lat;
		estado.camera.dir_long = rad(-90);
		estado.camera.dir_lat = lim;*/
		//estado.camera.dist = 100;
		vista_atual = "vista_superior";
		printf("Vista atual: vista_superior\n");
		glutPostRedisplay();
	}
	else if (vista_atual == "vista_superior")
	{
		/* Terceira Pessoa */
		//estado.camera.dir_long = rad(180);
		//estado.camera.dir_lat = rad(25);
		//estado.camera.dist = 8;
		/*estado.camera.dir_long = longitude;
		estado.camera.dir_lat = latitude*/;
	vista_atual = "terceira_pessoa";
	printf("Vista atual: terceira_pessoa\n");
	glutPostRedisplay();
	}
}

void Special(int key, int x, int y){
	switch (key) {
	case GLUT_KEY_F1: alteraVista();
		break;
	case GLUT_KEY_UP: estado.teclas.up = GL_TRUE;
		break;
	case GLUT_KEY_DOWN: estado.teclas.down = GL_TRUE;
		break;
	case GLUT_KEY_LEFT: estado.teclas.left = GL_TRUE;
		break;
	case GLUT_KEY_RIGHT: estado.teclas.right = GL_TRUE;
		break;
	}
}
void SpecialKeyUp(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: estado.teclas.up = GL_FALSE;
		break;
	case GLUT_KEY_DOWN: estado.teclas.down = GL_FALSE;
		break;
	case GLUT_KEY_LEFT: estado.teclas.left = GL_FALSE;
		break;
	case GLUT_KEY_RIGHT: estado.teclas.right = GL_FALSE;
		break;
	}
}

void setProjection(int x, int y, GLboolean picking){
	glLoadIdentity();
	if (picking) {
		GLint vport[4];
		glGetIntegerv(GL_VIEWPORT, vport);
		gluPickMatrix(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 4, 4, vport);
	}
	//cout << estado.camera.fov << endl;
	gluPerspective(estado.camera.fov, (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 1, 500);

}

void myReshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	setProjection(0, 0, GL_FALSE);
	resolucaoX = (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / 640;
	resolucaoY = (GLfloat)glutGet(GLUT_WINDOW_HEIGHT) / 480;
	glMatrixMode(GL_MODELVIEW);
}


void motionRotate(int x, int y){
#define DRAG_SCALE	0.01
	double lim = M_PI / 2 - 0.1;
	estado.camera.dir_long += (estado.xMouse - x)*DRAG_SCALE;
	estado.camera.dir_lat -= (estado.yMouse - y)*DRAG_SCALE*0.5;
	if (estado.camera.dir_lat > lim)
		estado.camera.dir_lat = lim;
	else
		if (estado.camera.dir_lat < -lim)
			estado.camera.dir_lat = -lim;
	estado.xMouse = x;
	estado.yMouse = y;
	glutPostRedisplay();
}

void motionZoom(int x, int y){
#define ZOOM_SCALE	0.5
	estado.camera.dist -= (estado.yMouse - y)*ZOOM_SCALE;
	if (estado.camera.dist < 5)
		estado.camera.dist = 5;
	else
		if (estado.camera.dist > 200)
			estado.camera.dist = 200;
	estado.yMouse = y;
	glutPostRedisplay();
}

void motionDrag(int x, int y){
	GLuint buffer[100];
	GLint vp[4];
	GLdouble proj[16], mv[16];
	int n;
	GLdouble newx, newy, newz;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // guarda a projecção
	glLoadIdentity();
	setProjection(x, y, GL_TRUE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	desenhaPlanoDrag(estado.eixoTranslaccao);

	n = glRenderMode(GL_RENDER);
	if (n > 0) {
		glGetIntegerv(GL_VIEWPORT, vp);
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		glGetDoublev(GL_MODELVIEW_MATRIX, mv);
		gluUnProject(x, glutGet(GLUT_WINDOW_HEIGHT) - y, (double)buffer[2] / UINT_MAX, mv, proj, vp, &newx, &newy, &newz);
		printf("Novo x:%lf, y:%lf, z:%lf\n\n", newx, newy, newz);
		switch (estado.eixoTranslaccao) {
		case EIXO_X:
			estado.eixo[0] = newx;
			//estado.eixo[1]=newy;
			break;
		case EIXO_Y:
			estado.eixo[1] = newy;
			//estado.eixo[2]=newz;
			break;
		case EIXO_Z:
			//estado.eixo[0]=newx;
			estado.eixo[2] = newz;
			break;
		}
		glutPostRedisplay();
	}


	glMatrixMode(GL_PROJECTION); //repõe matriz projecção
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

int picking(int x, int y){
	int i, n, objid = 0;
	double zmin = 10.0;
	GLuint buffer[100], *ptr;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // guarda a projecção
	glLoadIdentity();
	setProjection(x, y, GL_TRUE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	desenhaEixos();

	n = glRenderMode(GL_RENDER);
	if (n > 0)
	{
		ptr = buffer;
		for (i = 0; i < n; i++)
		{
			if (zmin >(double) ptr[1] / UINT_MAX) {
				zmin = (double)ptr[1] / UINT_MAX;
				objid = ptr[3];
			}
			ptr += 3 + ptr[0]; // ptr[0] contem o número de nomes (normalmente 1); 3 corresponde a numnomes, zmin e zmax
		}
	}


	glMatrixMode(GL_PROJECTION); //repõe matriz projecção
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	return objid;
}
void mouse(int btn, int state, int x, int y){
	switch (btn) {
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN){
			estado.xMouse = x;
			estado.yMouse = y;
			if (glutGetModifiers() & GLUT_ACTIVE_CTRL)
				glutMotionFunc(motionZoom);
			else
				glutMotionFunc(motionRotate);
			cout << "Left down\n";
		}
		else{
			glutMotionFunc(NULL);
			cout << "Left up\n";
		}
		break;
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN){
			estado.xMouse = x;
			cout << estado.xMouse << endl;
			int a = abs(y - glutGet(GLUT_WINDOW_HEIGHT));
			estado.yMouse = a;
			estado.mouseState = 0;
			cout << estado.yMouse << endl;
			estado.eixoTranslaccao = picking(x, y);
			if (estado.eixoTranslaccao)
				glutMotionFunc(motionDrag);
			cout << "Right down - objecto:" << estado.eixoTranslaccao << endl;
		}
		else{
			if (estado.eixoTranslaccao != 0) {
				estado.camera.center[0] = estado.eixo[0];
				estado.camera.center[1] = estado.eixo[1];
				estado.camera.center[2] = estado.eixo[2];
				glutMotionFunc(NULL);
				estado.eixoTranslaccao = 0;
				glutPostRedisplay();
			}
			cout << "Right up\n";
			estado.mouseState = 1;
		}
		break;
	}
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	mdlviewer_init("door.mdl", portas.door);
	mdlviewer_init("barel.mdl", barris.barel);
	/* need both double buffering and z buffer */
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	Window.Main = glutCreateWindow("OpenGL");

	//carrega modelo para a variavel modelo.homer
	mdlviewer_init("door.mdl", portas.door);
	mdlviewer_init("barel.mdl", barris.barel);
	mdlviewer_init("fimMundo.mdl", fimMundo.modelo);
	mdlviewer_init(nomeMDLjugador1, modelo.homer);


	glutReshapeFunc(myReshape);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(Special);
	glutSpecialUpFunc(SpecialKeyUp);
	glutMouseFunc(mouse);

	myInit();

	alutInit(&argc, argv);
	alGenBuffers(1, &estado.buffer);
	alGenSources(1, &estado.source);
	alGenBuffers(1, &estado.bufferExplosao);
	alGenSources(1, &estado.sourceExplosao);
	alGenBuffers(1, &estado.bufferDonut);
	alGenSources(1, &estado.sourceDonut);
	InitAudio("musica_fundo");


	initFimNivel();




	glutTimerFunc(estado.delay, Timer, 0);

	initLogin();
	initMenu();

	createTextures(modelo.texID);

	imprime_ajuda();

	//inicializacao das variaveis de minimapa
	Window.Top = glutCreateSubWindow(Window.Main, 1, 1, 1, 1);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glutDisplayFunc(DesenhaMinimapa);

	glutMainLoop();
}
