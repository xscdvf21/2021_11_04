// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>


//for.Client 
// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <functional>
#include <fstream>
#include <iostream>
using namespace std;

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "Typedef.h"
#include "Define.h"
#include "Extern.h"
#include "Function.h"
#include "Struct.h" 

// ����
#include <io.h>
#include "fmod.h"
#pragma comment (lib, "fmodex_vc.lib")
//////////////////////////////////////////////////

// ������
#include <Vfw.h>
#pragma comment (lib, "vfw32.lib")
//////////////////////////////////////////////////

#include "Graphic_Device.h"
