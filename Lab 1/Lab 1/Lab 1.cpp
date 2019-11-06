
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string>
#include <future>

using namespace std;


void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}


void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

int updateResult(HANDLE hFile, char lpBuffer[], DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped, std::atomic_bool& cancelled)
{
	int result = -1;
	
	while (!cancelled)
	{
		ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
		int result = atoi(lpBuffer);
		return result;
	}

	return result;
	
}


bool CheckEscape(HANDLE hFileF, char lpBufferF[], DWORD nNumberOfBytesToReadF, LPDWORD lpNumberOfBytesReadF, LPOVERLAPPED lpOverlappedF, int* ValueF, int* ValueG,
	HANDLE hFileG, char lpBufferG[], DWORD nNumberOfBytesToReadG, LPDWORD lpNumberOfBytesReadG, LPOVERLAPPED lpOverlappedG) {

	if (GetAsyncKeyState(VK_ESCAPE) == -32767) {


		HideConsole();
		PROCESS_INFORMATION piProcInfo;
		STARTUPINFO SI;
		wstring CommandLine(L"Cancel_process.exe");
		LPTSTR ClientName = &CommandLine[0];
		DWORD excode;

		ZeroMemory(&SI, sizeof(STARTUPINFO));
		SI.cb = sizeof(STARTUPINFO);
		ZeroMemory(&piProcInfo, sizeof(piProcInfo));

		if ((CreateProcess(NULL, ClientName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &SI, &piProcInfo)) == 0)
		{
			printf("create cancelation process: error code %d\n", (int)GetLastError());
		}

		//future<int> tmpf = std::async(std::launch::async, [&]() {return updateResult(hFileF, lpBufferF, nNumberOfBytesToReadF, lpNumberOfBytesReadF,lpOverlappedF); });
		//future<int> tmpg = std::async(std::launch::async, [&]() {return updateResult(hFileG, lpBufferG, nNumberOfBytesToReadG, lpNumberOfBytesReadG, lpOverlappedG); });


		bool flagF = false;
		bool flagG = false;

		while (true)
		{
			Sleep(10);
			GetExitCodeProcess(piProcInfo.hProcess, &excode);

			chrono::microseconds span(10);

			/*if (!flagF && tmpf.wait_for(span) == future_status::ready)
			{
				*ValueF = tmpf.get();
				flagF = true;
			}

			if (!flagG && tmpg.wait_for(span) == future_status::ready)
			{
				*ValueG = tmpg.get();
				flagG = true;
			}*/
			

			if (GetAsyncKeyState(0x59) == -32767 || excode != STILL_ACTIVE)
			{
				ShowConsole();
				std::cout << endl << "Abortion complete" << endl;

				if (*ValueF != -1)
					cout << "Function F computed: " << *ValueF << ", function g not" << endl;

				if (*ValueG != -1)
					cout << "Function G computed: " << *ValueG << ", function f not" << endl;

				TerminateProcess(piProcInfo.hProcess, 0);
				return true;
			}
			if (GetAsyncKeyState(0x4E) == -32767)
			{
				TerminateProcess(piProcInfo.hProcess, 0);
				ShowConsole();
				std::cout << endl << "Abortion stopped by user!" << endl;
				break;
			}
		}
	}

	return false;

}

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



void computing()
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

	DWORD NumBytesToWriteToCliForG;
	DWORD NumBytesToWriteToCli;

	hPipeF = CreateNamedPipeForFunctions(piProcInfoF, SIF, hPipeF, PipeNameF);

	CreateProcessForFunctions(piProcInfoF, SIF, ClientNameF);

	if ((ConnectNamedPipe(hPipeF, NULL)) == 0)
	{
		printf("client F could not connect\n");
	}

	hPipeG = CreateNamedPipeForFunctions(piProcInfoG, SIG, hPipeG, PipeNameG);

	CreateProcessForFunctions(piProcInfoG, SIG, ClientNameG);

	if ((ConnectNamedPipe(hPipeG, NULL)) == 0)
	{
		printf("client G could not connect\n");
	}

	int x = 0;
	int ValueF = -1;
	int ValueG = -1;

	std::cout << "Enter value to calculate : ";
	cin >> x;

	string tmp = to_string(x);
	const char* BuffToClient = tmp.c_str();

	WriteFile(hPipeF, BuffToClient, strlen(BuffToClient), &NumBytesToWriteToCli, NULL);

	WriteFile(hPipeG, BuffToClient, strlen(BuffToClient), &NumBytesToWriteToCliForG, NULL);

	std::atomic_bool cancellation_tokenF = false;
	std::atomic_bool cancellation_tokenG = false;
	

	future<int> tmpf = std::async(std::launch::async, [&]() {return updateResult(hPipeF, BuffToReadF, iNumBytesToReadF, &iNumBytesToReadF, NULL, std::ref(cancellation_tokenF)); });
	future<int> tmpg = std::async(std::launch::async, [&]() {return updateResult(hPipeG, BuffToReadG, iNumBytesToReadG, &iNumBytesToReadG, NULL, std::ref(cancellation_tokenG)); });

	bool flagF = false;
	bool flagG = false;

	while (true)
	{

		chrono::microseconds span(10);
		if (!flagF && tmpf.wait_for(span) == future_status::ready)
		{

			ValueF = tmpf.get();
			flagF = true;
		}


		if (ValueF == 0)
		{
			std::cout << "Value f = 0, stop computing." << endl;
			cancellation_tokenF = true;
			cancellation_tokenG = true;
			ValueG = 1;
			break;
		}


		if (!flagG && tmpg.wait_for(span) == future_status::ready)
		{
			ValueG = tmpg.get();

			flagG = true;
		}

		if (ValueG == 0)
		{
			std::cout << "Value g = 0, stop computing." << endl;
			cancellation_tokenF = true;
			cancellation_tokenG = true;
			ValueF = 1;

			break;
		}

		if (ValueF != -1 && ValueG != -1)
		{
			std::cout << "Value f = " << ValueF << ", Value G = " << ValueG << endl;
			break;
		}
		if (CheckEscape(hPipeF, BuffToReadF, iNumBytesToReadF, &iNumBytesToReadF, NULL, &ValueF, &ValueG, hPipeG, BuffToReadG, iNumBytesToReadG, &iNumBytesToReadG, NULL))
			return;
	}

	int minimum = MinimumFunction(ValueF, ValueG);
	std::cout << "Minimum value is: " << minimum << endl;

}
int main()
{
	while (true)
	{
		computing();
		std::cout << endl;
	}
}
