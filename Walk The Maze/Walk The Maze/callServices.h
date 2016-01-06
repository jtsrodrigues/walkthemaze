#ifndef _CALL_INCLUDE
#define _CALL_INCLUDE
#include <string>
#include <iostream>
using namespace std;

string callService_DownloadMapa(string mapa);
bool callService_CheckLogin(string user, string password);
string wstringToString(const wstring& s);
bool callService_InsertTrack(string user, string map, string track);
string callService_InfoMapa();
string callService_DownloadMusica(string nome);
string callService_DownloadTextura(string nome);
bool callService_UploadPontuacao(string user, int score, string mapa);
string callService_DownloadModeloInimigo(string nome);
#endif