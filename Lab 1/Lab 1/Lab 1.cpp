
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

using namespace std;
int main()
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO SI;
	wstring CommandLine(L"function_f.exe");
	LPTSTR ClientName = &CommandLine[0];
	HANDLE hPipe;
	wstring PipeAdress(L"\\\\.\\pipe\\MyPipe");
	LPTSTR PipeName = &PipeAdress[0];
	char Buff[255];
	DWORD iNumBytesToRead = 255, i;


	PROCESS_INFORMATION piProcInfoG;
	STARTUPINFO SIG;
	wstring CommandLineG(L"function_g.exe");
	LPTSTR ClientNameG = &CommandLineG[0];
	HANDLE hPipeG;
	wstring PipeAdressForForFunctionG(L"\\\\.\\pipe\\MyPipeForG");
	LPTSTR PipeNameG = &PipeAdressForForFunctionG[0];
	char BuffG[255];
	DWORD iNumBytesToReadG = 255, k;


	ZeroMemory(&SI, sizeof(STARTUPINFO));
	SI.cb = sizeof(STARTUPINFO);
	ZeroMemory(&piProcInfo, sizeof(piProcInfo));

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

	if ((CreateProcess(NULL, ClientName, NULL, NULL, FALSE, 0, NULL, NULL, &SI, &piProcInfo)) == 0)
	{
		printf("create client process: error code %d\n", (int)GetLastError());
	}

	if ((ConnectNamedPipe(hPipe, NULL)) == 0)
	{
		printf("client F could not connect\n");
	}

	ZeroMemory(&SIG, sizeof(STARTUPINFO));
	SIG.cb = sizeof(STARTUPINFO);
	ZeroMemory(&piProcInfo, sizeof(piProcInfo));

	hPipeG = CreateNamedPipe(
		PipeNameG,			   // имя канала
		PIPE_ACCESS_DUPLEX,       // чтение и запись из канала
		PIPE_TYPE_MESSAGE |       // передача сообщений по каналу
		PIPE_READMODE_MESSAGE |   // режим чтения сообщений 
		PIPE_WAIT,                // синхронная передача сообщений 
		PIPE_UNLIMITED_INSTANCES, // число экземпляров канала 
		4096,			   // размер выходного буфера
		4096,		          // размер входного буфера  
		NMPWAIT_USE_DEFAULT_WAIT, // тайм-аут клиента 
		NULL);                    // защита по умолчанию

	if (hPipeG == INVALID_HANDLE_VALUE)
	{
		printf("CreatePipe failed: error code %d\n", (int)GetLastError());
	}

	if ((CreateProcess(NULL, ClientNameG, NULL, NULL, FALSE, 0, NULL, NULL, &SIG, &piProcInfoG)) == 0)
	{
		printf("create client G process: error code %d\n", (int)GetLastError());
	}

	if ((ConnectNamedPipe(hPipeG, NULL)) == 0)
	{
		printf("client G could not connect\n");
	}

	

	char BuffToClient[] = "Message from server For F";
	char BuffToClientG[] = "Message from server For G";
	DWORD NumBytesToWriteToCliForG;
	DWORD NumBytesToWriteToCli;
	WriteFile(hPipe, BuffToClient, strlen(BuffToClient), &NumBytesToWriteToCli, NULL);
	_getch();
	WriteFile(hPipeG, BuffToClientG, strlen(BuffToClientG), &NumBytesToWriteToCliForG, NULL);


	

}
