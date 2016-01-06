#include "WebServices.h"
#include "schemas.microsoft.com.2003.10.Serialization.xsd.h"
#include "tempuri.org.xsd.h"
#include "tempuri.org.wsdl.h"
#include "callServices.h"
#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;
string callService_DownloadMapa(string mapa)
{
	HRESULT hr = ERROR_SUCCESS;
	WS_ERROR* error = NULL;
	WS_HEAP* heap = NULL;
	WS_SERVICE_PROXY* proxy = NULL;
	WS_ENDPOINT_ADDRESS address = {};
	// endereço do serviço
	WS_STRING url = WS_STRING_VALUE(L"http://wvm038.dei.isep.ipp.pt/website/ServiceSF.svc");
	address.url = url;
	hr = WsCreateHeap(2048, 512, NULL, 0, &heap, error);
	WS_HTTP_BINDING_TEMPLATE templ = {};
	// criação do proxy para o serviço
	hr = BasicHttpBinding_IServiceSF_CreateServiceProxy(&templ, NULL, 0, &proxy, error);
	hr = WsOpenServiceProxy(proxy, &address, NULL, error);
	WCHAR* result;
	// chamada de uma operação do serviço (Add)
	// o resultado vem no parâmetro result
	std::wstring wsTmp(mapa.begin(), mapa.end());
	wstring wString = wsTmp;
	WCHAR * filename = &wString[0];

	hr = BasicHttpBinding_IServiceSF_DownloadMapa(proxy, filename, &result, heap, NULL, 0, NULL, error);
	//wprintf(L"%s\n", result);

	wstring a = wstring(result);
	//string s((const char*)&a[0], sizeof(wchar_t) / sizeof(char)*a.size());

	string s = wstringToString(a);
	
	if (proxy)
	{
		WsCloseServiceProxy(proxy, NULL, NULL);
		WsFreeServiceProxy(proxy);
	}
	if (heap)
	{
		WsFreeHeap(heap);
	}
	if (error)
	{
		WsFreeError(error);
	}
	return s;
}

bool callService_CheckLogin(string user, string password)
{

	cout << user << " " << password << endl;
	HRESULT hr = ERROR_SUCCESS;
	WS_ERROR* error = NULL;
	WS_HEAP* heap = NULL;
	WS_SERVICE_PROXY* proxy = NULL;
	WS_ENDPOINT_ADDRESS address = {};
	// endereço do serviço
	WS_STRING url = WS_STRING_VALUE(L"http://wvm038.dei.isep.ipp.pt/website/ServiceSF.svc");
	address.url = url;
	hr = WsCreateHeap(2048, 512, NULL, 0, &heap, error);
	WS_HTTP_BINDING_TEMPLATE templ = {};
	// criação do proxy para o serviço
	hr = BasicHttpBinding_IServiceSF_CreateServiceProxy(&templ, NULL, 0, &proxy, error);
	hr = WsOpenServiceProxy(proxy, &address, NULL, error);
	BOOL result;
	result = true;
	// chamada de uma operação do serviço (Add)
	// o resultado vem no parâmetro result
	std::wstring wsTmp(user.begin(), user.end());
	wstring wString = wsTmp;
	WCHAR * username = &wString[0];

	std::wstring wsTmp2(password.begin(), password.end());
	wstring wString2 = wsTmp2;
	WCHAR * pass = &wString2[0];
	cout << "result: " << result << endl;
	hr = BasicHttpBinding_IServiceSF_CheckLogin(proxy, username, pass, &result, heap, NULL, 0, NULL, error);
	cout << "result: " << result << endl;
	if (proxy)
	{
		WsCloseServiceProxy(proxy, NULL, NULL);
		WsFreeServiceProxy(proxy);
	}
	if (heap)
	{
		WsFreeHeap(heap);
	}
	if (error)
	{
		WsFreeError(error);
	}

	bool res = result;
	return res;
}

string wstringToString(const wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
	std::string r(buf);
	delete[] buf;
	return r;
}

bool callService_InsertTrack(string user, string map,string track)
{
	HRESULT hr = ERROR_SUCCESS;
	WS_ERROR* error = NULL;
	WS_HEAP* heap = NULL;
	WS_SERVICE_PROXY* proxy = NULL;
	WS_ENDPOINT_ADDRESS address = {};
	// endereço do serviço
	WS_STRING url = WS_STRING_VALUE(L"http://wvm038.dei.isep.ipp.pt/website/ServiceSF.svc");
	address.url = url;
	hr = WsCreateHeap(2048, 512, NULL, 0, &heap, error);
	WS_HTTP_BINDING_TEMPLATE templ = {};
	// criação do proxy para o serviço
	hr = BasicHttpBinding_IServiceSF_CreateServiceProxy(&templ, NULL, 0, &proxy, error);
	hr = WsOpenServiceProxy(proxy, &address, NULL, error);
	BOOL result;
	result = true;
	// chamada de uma operação do serviço (Add)
	// o resultado vem no parâmetro result
	std::wstring wsTmp(user.begin(), user.end());
	wstring wString = wsTmp;
	WCHAR * username = &wString[0];

	std::wstring wsTmp2(map.begin(), map.end());
	wstring wString2 = wsTmp2;
	WCHAR * mapa = &wString2[0];
	std::wstring wsTmp3(track.begin(), track.end());
	wstring wString3 = wsTmp3;
	WCHAR * percurso = &wString3[0];

	hr = BasicHttpBinding_IServiceSF_InsertTrack(proxy, username, mapa, percurso, &result, heap, NULL, 0, NULL, error);
	cout << "result: " << result << endl;
	if (proxy)
	{
		WsCloseServiceProxy(proxy, NULL, NULL);
		WsFreeServiceProxy(proxy);
	}
	if (heap)
	{
		WsFreeHeap(heap);
	}
	if (error)
	{
		WsFreeError(error);
	}

	bool res = result;
	return res;
}

string callService_InfoMapa()
{
	HRESULT hr = ERROR_SUCCESS;
	WS_ERROR* error = NULL;
	WS_HEAP* heap = NULL;
	WS_SERVICE_PROXY* proxy = NULL;
	WS_ENDPOINT_ADDRESS address = {};
	// endereço do serviço
	WS_STRING url = WS_STRING_VALUE(L"http://wvm038.dei.isep.ipp.pt/website/ServiceSF.svc");
	address.url = url;
	hr = WsCreateHeap(2048, 512, NULL, 0, &heap, error);
	WS_HTTP_BINDING_TEMPLATE templ = {};
	// criação do proxy para o serviço
	hr = BasicHttpBinding_IServiceSF_CreateServiceProxy(&templ, NULL, 0, &proxy, error);
	hr = WsOpenServiceProxy(proxy, &address, NULL, error);
	WCHAR* result;
	// chamada de uma operação do serviço (Add)
	// o resultado vem no parâmetro result

	hr = BasicHttpBinding_IServiceSF_InfoMapa(proxy, &result, heap, NULL, 0, NULL, error);
	//wprintf(L"%s\n", result);

	wstring a = wstring(result);
	//string s((const char*)&a[0], sizeof(wchar_t) / sizeof(char)*a.size());

	string s = wstringToString(a);

	if (proxy)
	{
		WsCloseServiceProxy(proxy, NULL, NULL);
		WsFreeServiceProxy(proxy);
	}
	if (heap)
	{
		WsFreeHeap(heap);
	}
	if (error)
	{
		WsFreeError(error);
	}
	return s;
}

string callService_DownloadMusica(string nome)
{
	HRESULT hr = ERROR_SUCCESS;
	WS_ERROR* error = NULL;
	WS_HEAP* heap = NULL;
	WS_SERVICE_PROXY* proxy = NULL;
	WS_ENDPOINT_ADDRESS address = {};
	// endereço do serviço
	WS_STRING url = WS_STRING_VALUE(L"http://wvm038.dei.isep.ipp.pt/website/ServiceSF.svc");
	address.url = url;
	hr = WsCreateHeap(2048, 512, NULL, 0, &heap, error);
	WS_HTTP_BINDING_TEMPLATE templ = {};
	// criação do proxy para o serviço
	hr = BasicHttpBinding_IServiceSF_CreateServiceProxy(&templ, NULL, 0, &proxy, error);
	hr = WsOpenServiceProxy(proxy, &address, NULL, error);
	WCHAR* result;
	// chamada de uma operação do serviço (Add)
	// o resultado vem no parâmetro result
	std::wstring wsTmp(nome.begin(), nome.end());
	wstring wString = wsTmp;
	WCHAR * filename = &wString[0];

	hr = BasicHttpBinding_IServiceSF_DownloadMusica(proxy, filename, &result, heap, NULL, 0, NULL, error);
	//wprintf(L"%s\n", result);

	wstring a = wstring(result);
	//string s((const char*)&a[0], sizeof(wchar_t) / sizeof(char)*a.size());

	string s = wstringToString(a);

	if (proxy)
	{
		WsCloseServiceProxy(proxy, NULL, NULL);
		WsFreeServiceProxy(proxy);
	}
	if (heap)
	{
		WsFreeHeap(heap);
	}
	if (error)
	{
		WsFreeError(error);
	}
	return s;
}

string callService_DownloadModeloInimigo(string nome)
{
	HRESULT hr = ERROR_SUCCESS;
	WS_ERROR* error = NULL;
	WS_HEAP* heap = NULL;
	WS_SERVICE_PROXY* proxy = NULL;
	WS_ENDPOINT_ADDRESS address = {};
	// endereço do serviço
	WS_STRING url = WS_STRING_VALUE(L"http://wvm038.dei.isep.ipp.pt/website/ServiceSF.svc");
	address.url = url;
	hr = WsCreateHeap(2048, 512, NULL, 0, &heap, error);
	WS_HTTP_BINDING_TEMPLATE templ = {};
	// criação do proxy para o serviço
	hr = BasicHttpBinding_IServiceSF_CreateServiceProxy(&templ, NULL, 0, &proxy, error);
	hr = WsOpenServiceProxy(proxy, &address, NULL, error);
	WCHAR* result;
	// chamada de uma operação do serviço (Add)
	// o resultado vem no parâmetro result
	std::wstring wsTmp(nome.begin(), nome.end());
	wstring wString = wsTmp;
	WCHAR * filename = &wString[0];

	hr = BasicHttpBinding_IServiceSF_DownloadModeloInimigo(proxy, filename, &result, heap, NULL, 0, NULL, error);
	//wprintf(L"%s\n", result);

	wstring a = wstring(result);
	//string s((const char*)&a[0], sizeof(wchar_t) / sizeof(char)*a.size());

	string s = wstringToString(a);

	if (proxy)
	{
		WsCloseServiceProxy(proxy, NULL, NULL);
		WsFreeServiceProxy(proxy);
	}
	if (heap)
	{
		WsFreeHeap(heap);
	}
	if (error)
	{
		WsFreeError(error);
	}
	return s;
}
string callService_DownloadTextura(string nome)
{
	HRESULT hr = ERROR_SUCCESS;
	WS_ERROR* error = NULL;
	WS_HEAP* heap = NULL;
	WS_SERVICE_PROXY* proxy = NULL;
	WS_ENDPOINT_ADDRESS address = {};
	// endereço do serviço
	WS_STRING url = WS_STRING_VALUE(L"http://wvm038.dei.isep.ipp.pt/website/ServiceSF.svc");
	address.url = url;
	hr = WsCreateHeap(2048, 512, NULL, 0, &heap, error);
	WS_HTTP_BINDING_TEMPLATE templ = {};
	// criação do proxy para o serviço
	hr = BasicHttpBinding_IServiceSF_CreateServiceProxy(&templ, NULL, 0, &proxy, error);
	hr = WsOpenServiceProxy(proxy, &address, NULL, error);
	WCHAR* result;
	// chamada de uma operação do serviço (Add)
	// o resultado vem no parâmetro result
	std::wstring wsTmp(nome.begin(), nome.end());
	wstring wString = wsTmp;
	WCHAR * filename = &wString[0];

	hr = BasicHttpBinding_IServiceSF_DownloadTextura(proxy, filename, &result, heap, NULL, 0, NULL, error);
	//wprintf(L"%s\n", result);

	wstring a = wstring(result);
	//string s((const char*)&a[0], sizeof(wchar_t) / sizeof(char)*a.size());

	string s = wstringToString(a);

	if (proxy)
	{
		WsCloseServiceProxy(proxy, NULL, NULL);
		WsFreeServiceProxy(proxy);
	}
	if (heap)
	{
		WsFreeHeap(heap);
	}
	if (error)
	{
		WsFreeError(error);
	}
	return s;
}

bool callService_UploadPontuacao(string user, int score, string mapa)
{
	HRESULT hr = ERROR_SUCCESS;
	WS_ERROR* error = NULL;
	WS_HEAP* heap = NULL;
	WS_SERVICE_PROXY* proxy = NULL;
	WS_ENDPOINT_ADDRESS address = {};
	// endereço do serviço
	WS_STRING url = WS_STRING_VALUE(L"http://wvm038.dei.isep.ipp.pt/website/ServiceSF.svc");
	address.url = url;
	hr = WsCreateHeap(2048, 512, NULL, 0, &heap, error);
	WS_HTTP_BINDING_TEMPLATE templ = {};
	// criação do proxy para o serviço
	hr = BasicHttpBinding_IServiceSF_CreateServiceProxy(&templ, NULL, 0, &proxy, error);
	hr = WsOpenServiceProxy(proxy, &address, NULL, error);
	BOOL result;
	result = true;
	// chamada de uma operação do serviço (Add)
	// o resultado vem no parâmetro result
	std::wstring wsTmp(user.begin(), user.end());
	wstring wString = wsTmp;
	WCHAR * username = &wString[0];

	std::wstring wsTmp2(mapa.begin(), mapa.end());
	wstring wString2 = wsTmp2;
	WCHAR * mapa2 = &wString2[0];

	hr = BasicHttpBinding_IServiceSF_InsertUploadData(proxy, username,score, mapa2, &result, heap, NULL, 0, NULL, error);
	cout << "result: " << result << endl;
	if (proxy)
	{
		WsCloseServiceProxy(proxy, NULL, NULL);
		WsFreeServiceProxy(proxy);
	}
	if (heap)
	{
		WsFreeHeap(heap);
	}
	if (error)
	{
		WsFreeError(error);
	}

	bool res = result;
	return res;
}