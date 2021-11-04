#pragma once

typedef struct tagTexInfo
{
	LPDIRECT3DTEXTURE9 pTexture; // hbitmap  과 같은 역할. 텍스쳐 한장을 제어하기 위한 컴객체. 
	D3DXIMAGE_INFO tImageInfo; // 이미지의 정보를 가지고 있을 구조체. 
}TEXINFO;
