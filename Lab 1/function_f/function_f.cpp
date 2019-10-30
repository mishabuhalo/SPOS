// function_f.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <string>

using namespace std;

int main()
{

	HANDLE hPipe;
	wstring PipeAdress(L"\\\\.\\pipe\\MyPipe");
	LPTSTR PipeName = &PipeAdress[0];
	DWORD NumBytesToWrite;
	char Buff[] = "Message from Client";
	char BuffForServer[255];
	DWORD NumBytesToRead =255, i;

	hPipe = CreateFile(
		PipeName,	      // имя канала
		GENERIC_READ |  // чтение и запись в канал
		GENERIC_WRITE,
		0,              // нет разделяемых операций 
		NULL,           // защита по умолчанию
		OPEN_EXISTING,  // открытие существующего канала 
		0,              // атрибуты по умолчанию
		NULL);          // нет дополнительных атрибутов 

	ReadFile(hPipe, BuffForServer, NumBytesToRead, &NumBytesToRead, NULL);
	cout << "Child F recived from server: ";
	int Value = atoi(BuffForServer);

	cout << Value << endl;

	Value = Value * 5 - 10;

	string tmp = to_string(Value);
	const char *Result = tmp.c_str();
	
	WriteFile(hPipe, Result, strlen(Result), &NumBytesToWrite, NULL);


	
}
