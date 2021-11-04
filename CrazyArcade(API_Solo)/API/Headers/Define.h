#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

#define WINCX 760
#define WINCY 520

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }

#define OBJ_DEAD 1
#define OBJ_NOEVENT 0
#define OBJ_DELETE 1

#define PI 3.1415f

#define TILEX 30
#define TILEY 20
#define TILECX 40
#define TILECY 40

#endif // !__DEFINE_H__
