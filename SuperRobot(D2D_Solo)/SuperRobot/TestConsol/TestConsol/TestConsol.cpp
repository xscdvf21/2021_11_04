// TestConsol.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <windows.h>

int main()
{
	LARGE_INTEGER CpuTick; 
	LARGE_INTEGER BeginTime; 
	LARGE_INTEGER EndTime; 
	float fDeltatime = 0.f; 
	QueryPerformanceFrequency(&CpuTick);// CPU�� �ʴ� �������� ��ȯ�ϴ� �Լ�. 
	QueryPerformanceCounter(&BeginTime);
	QueryPerformanceCounter(&EndTime);

	while (true)
	{
		QueryPerformanceFrequency(&CpuTick);

		system("pause");

		QueryPerformanceCounter(&EndTime); 
		//�� ������ - ���� ������ / �ʴ� cpu ������ 
		fDeltatime = float(EndTime.QuadPart - BeginTime.QuadPart) / CpuTick.QuadPart; 
		BeginTime.QuadPart = EndTime.QuadPart; 
		cout << fDeltatime << endl; 

	}




	char szname[32] = "��abcd"; 
	int i = 0; 
	// -71, -38
	cout << (char)-71 /*<< (char)-38*/ << endl; 
    return 0;
}

