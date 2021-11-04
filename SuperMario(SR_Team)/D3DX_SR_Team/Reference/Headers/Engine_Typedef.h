#pragma once
#ifndef __ENGINE_TYPEDEF_H__

typedef bool _bool;

typedef char _char, _byte;
typedef unsigned char _uchar, _ubyte;

typedef int					_int;
typedef unsigned int		_uint;
typedef float				_float;

typedef D3DXVECTOR2			_float2;
typedef D3DXVECTOR3			_float3, _vec3;
typedef D3DXVECTOR4			_float4;
typedef D3DXMATRIX			_float4x4, _mat;
typedef LPDIRECT3DDEVICE9	_Device;

#define __ENGINE_TYPEDEF_H__
#endif