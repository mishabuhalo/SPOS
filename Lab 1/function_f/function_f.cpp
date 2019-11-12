// function_f.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <future>
#include "demofuncs"
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

	//cout << "Child F recived from server: ";
	int Value = atoi(BuffForServer);

	//cout << Value << endl;

	std::future<int> resultFuture = std::async(std::launch::async, [=]() { return spos::lab1::demo::f_func<spos::lab1::demo::INT>(Value); });

	while (resultFuture.wait_for(std::chrono::milliseconds(6000)) != std::future_status::ready)
	{
		if (ReadFile(hPipe, BuffForServer, NumBytesToRead, &NumBytesToRead, NULL))
		{
			WriteFile(hPipe, "1", 2, &NumBytesToWrite, NULL);
			return 0;
		}
	}
	

	string tmp = to_string(resultFuture.get());
	const char *Final = tmp.c_str();
	
	WriteFile(hPipe, Final, strlen(Final), &NumBytesToWrite, NULL);


	
}
