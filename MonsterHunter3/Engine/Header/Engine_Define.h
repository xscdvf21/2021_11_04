#ifndef Engine_Define_h__
#define Engine_Define_h__

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>

#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Enum.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"

#pragma warning(disable : 4251)

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 


#define OBJ_DEAD 1
#define OBJ_NOEVENT 0

#endif
#endif

using namespace std;
using namespace Engine;


#endif // Engine_Define_h__
