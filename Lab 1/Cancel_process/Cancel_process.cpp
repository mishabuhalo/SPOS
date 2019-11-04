

#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
	for (int i = 60; i >= 0; i--)
	{
		cout << '\r' << i<< " seconds before interruption of computing. Press (y) to cancel immediately or (n) to cancel";
		Sleep(1000);
	}
	cout << endl;
}

