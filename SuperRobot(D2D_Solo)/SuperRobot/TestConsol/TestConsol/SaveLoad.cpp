#include "stdafx.h"
#include <fstream>
void main()
{
	// �ھ� ���� ���̺�� �ε带 ������ ���ô�. 
	// �Ǵٸ� ���̺�� �ε� ���� ������ϴ°� �˷��ٷ���. 
// 	ofstream fout; 
// 	char szBuf[256] = "���������� �߱ο���� �縺��ƴ��Ҽ�"; 
// 	fout.open("../Data/Test.txt");
// 	if (fout.fail())
// 		cout << "���� ����!? " << endl; 
// 	fout << szBuf << endl; 
// 	fout.close(); 

	char szReadText[256]; 
	ifstream fin; 
	fin.open("../Data/Test.txt"); 

	if (fin.fail())
	{
		cout << "����!" << endl; 
	}
	fin.getline(szReadText, 256);
	cout << szReadText << endl; 
	
	//cout.

}