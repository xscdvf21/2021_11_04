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
#include <random>
#include <time.h>
#include <math.h>
#include "PxPhysicsAPI.h"
#include <locale.h>
#include <codecvt>
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Enum.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include <fstream>

#pragma warning(disable : 4251)

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#ifndef _TOOL
extern int g_FillMode;


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif
#endif

// FMOD
#ifndef _WIN64
#include "../../inc/fmod.hpp"
#pragma comment (lib, "../../lib/x86/fmod_vc.lib")
using namespace FMOD;
#endif
#ifdef _WIN64
#include <io.h>
#include "../../inc/fmod.hpp"
#pragma comment (lib, "../../lib/x64/fmod_vc.lib")
using namespace FMOD;
#endif

using namespace std;
using namespace Engine;
using namespace physx;

#endif // Engine_Define_h__
