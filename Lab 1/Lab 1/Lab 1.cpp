
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string>
using namespace std;
HANDLE CreateNamedPipeForFunctions(PROCESS_INFORMATION piProcInfo, STARTUPINFO SI, HANDLE hPipe, LPTSTR PipeName)
{
	

	hPipe = CreateNamedPipe(
		PipeName,			   // имя канала
		PIPE_ACCESS_DUPLEX,       // чтение и запись из канала
		PIPE_TYPE_MESSAGE |       // передача сообщений по каналу
		PIPE_READMODE_MESSAGE |   // режим чтения сообщений 
		PIPE_WAIT,                // синхронная передача сообщений 
		PIPE_UNLIMITED_INSTANCES, // число экземпляров канала 
		4096,			   // размер выходного буфера
		4096,		          // размер входного буфера  
		NMPWAIT_USE_DEFAULT_WAIT, // тайм-аут клиента 
		NULL);                    // защита по умолчанию

	if (hPipe == INVALID_HANDLE_VALUE)
	{
		printf("CreatePipe failed: error code %d\n", (int)GetLastError());
	}
	return hPipe;
}

void CreateProcessForFunctions(PROCESS_INFORMATION piProcInfo, STARTUPINFO SI, LPTSTR ClientName)
{
	if ((CreateProcess(NULL, ClientName, NULL, NULL, FALSE, 0, NULL, NULL, &SI, &piProcInfo)) == 0)
	{
		printf("create client process: error code %d\n", (int)GetLastError());
	}
}

int MinimumFunction(int ValueF, int ValueG)
{
	return min(ValueF, ValueG);
}
int main()
{
	PROCESS_INFORMATION piProcInfoF;
	STARTUPINFO SIF;
	wstring CommandLineF(L"function_f.exe");
	LPTSTR ClientNameF = &CommandLineF[0];
	HANDLE hPipeF = NULL;
	wstring PipeAdressF(L"\\\\.\\pipe\\MyPipe");
	LPTSTR PipeNameF = &PipeAdressF[0];
	char BuffToReadF[255];
	DWORD iNumBytesToReadF = 255, i;

	ZeroMemory(&SIF, sizeof(STARTUPINFO));
	SIF.cb = sizeof(STARTUPINFO);
	ZeroMemory(&piProcInfoF, sizeof(piProcInfoF));

	

	hPipeF = CreateNamedPipeForFunctions(piProcInfoF, SIF, hPipeF, PipeNameF);
	CreateProcessForFunctions(piProcInfoF, SIF, ClientNameF);

	if ((ConnectNamedPipe(hPipeF, NULL)) == 0)
	{
		printf("client F could not connect\n");
	}


	PROCESS_INFORMATION piProcInfoG;
	STARTUPINFO SIG;
	wstring CommandLineG(L"function_g.exe");
	LPTSTR ClientNameG = &CommandLineG[0];
	HANDLE hPipeG = NULL;
	wstring PipeAdressForForFunctionG(L"\\\\.\\pipe\\MyPipeForG");
	LPTSTR PipeNameG = &PipeAdressForForFunctionG[0];
	char BuffToReadG[255];
	DWORD iNumBytesToReadG = 255, k;

	ZeroMemory(&SIG, sizeof(STARTUPINFO));
	SIG.cb = sizeof(STARTUPINFO);
	ZeroMemory(&piProcInfoG, sizeof(piProcInfoG));

	hPipeG = CreateNamedPipeForFunctions(piProcInfoG, SIG, hPipeG, PipeNameG);
	CreateProcessForFunctions(piProcInfoG, SIG, ClientNameG);

	

	DWORD NumBytesToWriteToCliForG;
	DWORD NumBytesToWriteToCli;
	int x;
	while (true)
	{
		cout << "Enter value to calculate : ";
		cin >> x;

		string tmp = to_string(x);
		const char *BuffToClient = tmp.c_str();

		WriteFile(hPipeF, BuffToClient, strlen(BuffToClient), &NumBytesToWriteToCli, NULL);

		_getch();

		WriteFile(hPipeG, BuffToClient, strlen(BuffToClient), &NumBytesToWriteToCliForG, NULL);

		_getch();

		ReadFile(hPipeF, BuffToReadF, iNumBytesToReadF, &iNumBytesToReadF, NULL);
		int ValueF = atoi(BuffToReadF);
		cout << "Server recived from client F number: " << ValueF << endl;

		_getch();

		ReadFile(hPipeG, BuffToReadG, iNumBytesToReadG, &iNumBytesToReadG, NULL);
		int ValueG = atoi(BuffToReadG);
		cout << "Server recived from client G number: " << ValueG << endl;

		_getch();

		int minimum = MinimumFunction(ValueF, ValueG);
		cout << "Minimum value is: " << minimum << endl;
	}

}
