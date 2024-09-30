//===========================================================================================================================================================
// 
// block3D.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BLOCK3D_H_
#define _BLOCK3D_H_

#include "main.h"
#include "model.h"

class CBlock3D:public CModel
{
private:
	static inline const float MAX_SIZE = 20.0f;	// ブロックのサイズ
public:
	CBlock3D();
	~CBlock3D()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CBlock3D* Create(D3DXVECTOR3 pos);
private:
};

#endif // !_BLOCK3D_H_
