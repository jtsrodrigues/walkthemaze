#include "grafos.h"
#include "callServices.h"
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

//#define __GRAFO__FILE__LIMITES__ "grafo_limites.grafo"
//#define __GRAFO__FILE__PAREDES__ "grafo_paredes.grafo"

No nosMundo[_MAX_NOS_GRAFO];
Arco arcosMundo[_MAX_ARCOS_GRAFO];
int numNosMundo = 0, numArcosMundo = 0, numParedes = 0, numObstaculos = 15;

No nosQuarto[_MAX_NOS_GRAFO];
Arco arcosQuarto[_MAX_ARCOS_GRAFO];
Paredes paredes[_MAX_ARCOS_GRAFO];
Obstaculos obstaculos[_MAX_OBSTACULOS];
int numNosQuarto = 0, numArcosQuarto = 0,numRampasQuarto=0;
string mundoFile;
string quartoFile;
No rampas[_MAX_RAMPAS_GRAFO];;
No nosLuz[7];
int numLuzes = 0;

int tamanhoQuartoX = 0;
int tamanhoQuartoY = 0;
int quantidadeInimigosVisiveis=0;
int posxInimigos[100];
int posyInimigos[100];
int poszInimigos[100];

 int quantidadeAlcapoes=0;
 int posxalcapao[30];
 int posyalcapao[30];
 int poszalcapao[30];

 int quantidadeAlcapoesQuartoSecreto = 0;
 int posxalcapaoQuartoSecreto[30];
 int posyalcapaoQuartoSecreto[30];
 int poszalcapaoQuartoSecreto[30];
 int posxfinaisAlcapao[30];
 int posyfinaisAlcapao[30];

 int numPosicaoTrans = 0;
 double posxtrans[30];
 double posytrans[30];
 double porta[30];
 int posxfinais[30];
 int posyfinais[30];
 int proxquarto[30];
 int posxinicio[30];
 int posyinicio[30];
 int posxfinal[30];
 int posyfinal[30];

 int quantidadeObstaculos = 0;
 int posXObstaculos[50];
 int posYObstaculos[50];

 int pontuacao;

using namespace std;

void addNoMundo(No no){
	if (numNosMundo<_MAX_NOS_GRAFO){
		nosMundo[numNosMundo] = no;
		numNosMundo++;
	}
	else{
		cout << "Número de nós chegou ao limite" << endl;
	}
}

void addNoQuarto(No no){
	if (numNosQuarto<_MAX_NOS_GRAFO){
		nosQuarto[numNosQuarto] = no;
		numNosQuarto++;
	}
	else{
		cout << "Número de nós chegou ao limite" << endl;
	}
}

No criaNo(float x, float y, float z){
	No no;
	no.x = x;
	no.y = y;
	no.z = z;
	return no;
}

void addArcoMundo(Arco arco){
	if (numArcosMundo<_MAX_ARCOS_GRAFO){
		arcosMundo[numArcosMundo] = arco;
		numArcosMundo++;
	}
	else{
		cout << "Número de arcos chegou ao limite" << endl;
	}
}

void addArcoQuarto(Arco arco){
	if (numArcosQuarto<_MAX_ARCOS_GRAFO){
		arcosQuarto[numArcosQuarto] = arco;
		numArcosQuarto++;
	}
	else{
		cout << "Número de arcos chegou ao limite" << endl;
	}
}

Arco criaArco(int noi, int nof, float peso, float largura){
	Arco arco;
	arco.noi = noi;
	arco.nof = nof;
	arco.peso = peso;
	arco.largura = largura;
	return arco;
}

void setFicheiroMundo(string nome)
{
	mundoFile = nome;
}

void setFicheiroQuarto(string nome)
{
	quartoFile = nome;
}

vector<string> split(string str, char delimiter)
{
	vector<string> ret;
	if ((str.find(delimiter) == string::npos) && (str.find_first_not_of(delimiter) == string::npos)) throw nullptr;
	else if ((str.find(delimiter) == string::npos)) ret.push_back(str);
	else if (str.find_first_not_of(delimiter) == string::npos) ret.push_back(string(""));
	else
	{
		unsigned i = 0;
		string strstack;
		while (str[0] == delimiter) { str.erase(0, 1); }
		reverse(str.begin(), str.end());
		while (str[0] == delimiter) { str.erase(0, 1); }
		reverse(str.begin(), str.end());
		while (!str.empty())
		{
			ret.push_back(str.substr(i, str.find(delimiter)));
			str.erase(0, str.find(delimiter));
			while (str[0] == delimiter) { str.erase(0, 1); }
		}
	}
	return ret;
}

float converter(string num){
	std::istringstream iss(num);
	float f_val = 0;
	iss >> f_val;
	return f_val;
}



void leGrafoMundo(string mundo){
	int i;

	vector<string> mundo2 = split(mundo, '\n');
	ofstream file("mundo.txt");
	if (file.is_open())
	{
		for (i = 0; i < mundo2.size(); i++){
			file << mundo2[i]<<"\n";
		}
		file.close();
	}
	mundoFile = "mundo.txt";
	ifstream myfile;
	
	myfile.open(mundoFile, ios::in);
	if (!myfile.is_open()) {
		cout << "Erro ao abrir " << mundoFile << "para ler" << endl;
		exit(1);
	}
	myfile >> numNosMundo;
	for (i = 0; i < numNosMundo; i++){
		myfile >> nosMundo[i].x >> nosMundo[i].y >> nosMundo[i].z;
	}
	myfile >> numArcosMundo;
	for (i = 0; i < numArcosMundo; i++){
		myfile >> arcosMundo[i].noi >> arcosMundo[i].nof >> arcosMundo[i].peso >> arcosMundo[i].largura;
	}
	
	// calcula a largura de cada no = maior largura dos arcos que divergem/convergem desse/nesse no	
	for (int i = 0; i < numNosMundo; i++){
		nosMundo[i].largura = 0;
		for (int j = 0; j < numArcosMundo; j++)
			if ((arcosMundo[j].noi == i || arcosMundo[j].nof == i) && nosMundo[i].largura < arcosMundo[j].largura)
				nosMundo[i].largura = arcosMundo[j].largura;
	}
	myfile >> posxinicio[0] >> posyinicio[0];
	myfile >> posxfinal[0] >> posyfinal[0];
	myfile >> pontuacao;
	myfile.close();
	remove("mundo.txt");
}

void leGrafoQuarto(string quarto){
	int i;
	vector<string> quarto2 = split(quarto, '\n');
	ofstream file("quarto.txt");
	if (file.is_open())
	{
		for (i = 0; i < quarto2.size(); i++){
			file << quarto2[i] << "\n";
		}
		file.close();
	}
	quartoFile = "quarto.txt";

	ifstream myfile;

	myfile.open(quartoFile, ios::in);
	if (!myfile.is_open()) {
		cout << "Erro ao abrir " << quartoFile << "para ler" << endl;
		exit(1);
	}
	myfile >> numNosQuarto;
	for (int i = 0; i<numNosQuarto; i++)
		myfile >> nosQuarto[i].x >> nosQuarto[i].y >> nosQuarto[i].z;
	myfile >> numArcosQuarto;
	for (int i = 0; i < numArcosQuarto; i++)
		myfile >> arcosQuarto[i].noi >> arcosQuarto[i].nof >> arcosQuarto[i].peso >> arcosQuarto[i].largura;
	
	myfile >> numLuzes;
	//variavel de descartar luzes a mais
	int x;
	for (int i = 0; i < numLuzes; i++){
		if (i < 7){ myfile >> nosLuz[i].x >> nosLuz[i].y >> nosLuz[i].z >> nosLuz[i].largura; }
		//descartar acima da 7 luzes porque so tem 7 luzes o opengl para descartas as linhas do ficheiro ta aqui este else
		else{
			
			myfile >> x >> x >> x >> x;
		}
	}
	// calcula a largura de cada no = maior largura dos arcos que divergem/convergem desse/nesse no	
	for (int i = 0; i < numNosQuarto; i++){
		nosQuarto[i].largura = 0;
		for (int j = 0; j < numArcosQuarto; j++)
			if ((arcosQuarto[j].noi == i || arcosQuarto[j].nof == i) && nosQuarto[i].largura < arcosQuarto[j].largura)
				nosQuarto[i].largura = arcosQuarto[j].largura;

	}
	myfile >> tamanhoQuartoX;
	myfile >> tamanhoQuartoY;
	myfile >> numRampasQuarto;
	for (int i = 0; i < numRampasQuarto; i++)
		myfile >> rampas[i].x >> rampas[i].y >> rampas[i].z;


	myfile >> quantidadeInimigosVisiveis;
	if (quantidadeInimigosVisiveis>0){
		for (size_t i = 0; i < quantidadeInimigosVisiveis; i++)
		{
			myfile >> posxInimigos[i] >> posyInimigos[i] >> poszInimigos[i];
		}
	}
	myfile >> quantidadeAlcapoes;
	if (quantidadeAlcapoes>0){
		for (size_t i = 0; i < quantidadeAlcapoes; i++)
		{
			myfile >> posxalcapao[i] >> posyalcapao[i] >> poszalcapao[i];
		}
	}
	myfile >> quantidadeAlcapoesQuartoSecreto;
	if (quantidadeAlcapoesQuartoSecreto>0){
		for (size_t i = 0; i < quantidadeAlcapoesQuartoSecreto; i++)
		{
			myfile >> posxalcapaoQuartoSecreto[i] >> posyalcapaoQuartoSecreto[i] >> poszalcapaoQuartoSecreto[i];
			myfile >> posxfinaisAlcapao[i] >> posyfinaisAlcapao[i];
		}
	}
	myfile >> numPosicaoTrans;
	if (numPosicaoTrans>0){

		for (size_t i = 0; i < numPosicaoTrans; i++)
		{
			myfile >> posxtrans[i] >> posytrans[i]>>porta[i];
			myfile >> posxfinais[i] >> posyfinais[i];
			myfile >> proxquarto[i];
		}
	}

	//le obstaculos
	//myfile >> quantidadeObstaculos;
	myfile >> quantidadeObstaculos;
	//cout << "a: " << quantidadeObstaculos << endl;
	if (quantidadeObstaculos > 0){
		for (size_t i = 0; i < quantidadeObstaculos; i++)
		{
			myfile >> posXObstaculos[i] >> posYObstaculos[i];
		}
	}

	myfile.close();
	remove("quarto.txt");
}