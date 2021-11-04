#ifndef Client_Include_h__
#define Client_Include_h__

#include <d3d9.h>
#include <d3dx9.h>

//#include <vector>
//#include <list>
//#include <map>
//#include <unordered_map>
//#include <algorithm>
//#include <functional>
#include <ctime>
#include <string>
#include <memory>
#include "Trigger.h"
#include <process.h>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "ControllerCallback.h"
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;

#include "Client_Define.h"
#include "Client_Function.h"
#include "Engine_Define.h"

using namespace Engine;




#endif // Client_Include_h__

