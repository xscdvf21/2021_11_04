#ifndef Engine_Typedef_h__
#define Engine_Typedef_h__

namespace Engine
{
	typedef		bool					_bool;

	typedef		signed char				_char;
	typedef		signed char				_byte;
	typedef		unsigned char			_ubyte;

	typedef		wchar_t					_tchar;

	typedef		signed short			_short;
	typedef		unsigned short			_ushort;

	typedef		signed int				_int;
	typedef		unsigned int			_uint;

	typedef		signed long				_long;
	typedef		unsigned long			_ulong;

	typedef		float					_float;
	typedef		double					_double;

	typedef		D3DXVECTOR3				_vec3, _float3;
	typedef		D3DXVECTOR2				_vec2, _float2;
	typedef		D3DXVECTOR4				_vec4, _float4;

	typedef		D3DXMATRIX				_matrix, _float4x4, _mat;
	typedef		D3DXQUATERNION			_qtnian;
	typedef		std::wstring					_wstr;
}

#endif // Engine_Typedef_h__
