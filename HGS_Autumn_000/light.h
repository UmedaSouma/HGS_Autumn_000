//===========================================================================================================================================================
// 
// light.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

class CLight
{
private:
	static const unsigned int MAX_LIGHT = 3;	// ライトの最大数
public:
	CLight();
	~CLight();
	HRESULT Init();
	void Uninit();
	void Update();
private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];	// ライトの情報
	static int m_NumAll;	// ライトの数
};
#endif // !_LIGHT_H_
