// TestConsol.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <windows.h>

int main()
{
	LARGE_INTEGER CpuTick; 
	LARGE_INTEGER BeginTime; 
	LARGE_INTEGER EndTime; 
	float fDeltatime = 0.f; 
	QueryPerformanceFrequency(&CpuTick);// CPU의 초당 진동수를 반환하는 함수. 
	QueryPerformanceCounter(&BeginTime);
	QueryPerformanceCounter(&EndTime);

	while (true)
	{
		QueryPerformanceFrequency(&CpuTick);

		system("pause");

		QueryPerformanceCounter(&EndTime); 
		//끝 진동수 - 시작 진동수 / 초당 cpu 진동수 
		fDeltatime = float(EndTime.QuadPart - BeginTime.QuadPart) / CpuTick.QuadPart; 
		BeginTime.QuadPart = EndTime.QuadPart; 
		cout << fDeltatime << endl; 

	}




	char szname[32] = "박abcd"; 
	int i = 0; 
	// -71, -38
	cout << (char)-71 /*<< (char)-38*/ << endl; 
    return 0;
}

