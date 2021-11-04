#include "stdafx.h"
#include <fstream>
void main()
{
	// 자아 이제 세이브와 로드를 구현해 봅시다. 
	// 또다른 세이브와 로드 파일 입출력하는거 알려줄려고. 
// 	ofstream fout; 
// 	char szBuf[256] = "나랏말쌈이 뒹귁에닳아 사맏디아니할세"; 
// 	fout.open("../Data/Test.txt");
// 	if (fout.fail())
// 		cout << "성공 실패!? " << endl; 
// 	fout << szBuf << endl; 
// 	fout.close(); 

	char szReadText[256]; 
	ifstream fin; 
	fin.open("../Data/Test.txt"); 

	if (fin.fail())
	{
		cout << "나가!" << endl; 
	}
	fin.getline(szReadText, 256);
	cout << szReadText << endl; 
	
	//cout.

}