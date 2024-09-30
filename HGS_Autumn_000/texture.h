//===========================================================================================================================================================
// 
// texture.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//=======================================================================================================================
// クラス
//=======================================================================================================================
// テクスチャの管理クラス
class CTexture
{
private:
public:
	static const unsigned int MAX_TEXTURE = 256;
	CTexture();
	~CTexture();
	void Unload();
	int Regist(const char* pTexturename);
	LPDIRECT3DTEXTURE9 GetAddress(int Idx);

private:
	LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];
	const char* m_pTexStorage[MAX_TEXTURE];
	static int m_NumAll;
};

#endif // !_TEXTURE_H_
