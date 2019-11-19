

#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
	for (int i = 10; i >= 0; i--)
	{
		cout << '\r' << i<< " seconds before app will be stopped. Press (y) to stop app immediately or (n) to cancel";
		Sleep(1000);
	}
	cout << endl;
}

