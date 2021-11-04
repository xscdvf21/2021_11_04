#pragma once
#ifndef __ENGINE_INCLUDE_H__
#define __ENGINE_INCLUDE_H__

#pragma warning(disable : 4251)

/* Standard Headers */
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <memory>		//스마트 포인터
#include <assert.h>

#include <tchar.h>

using namespace std;

/* dx9 */
#include "d3d9.h"
#include "d3dx9.h"

/* User Headers */
#include "Engine_Define.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"

// 사운드
#include <map>
#include <io.h>
#include "fmod.h"
#pragma comment (lib, "fmodex_vc.lib")
////////////////////////////////////////////////


#endif