//===========================================================================================================================================================
// 
// object3D.cpp‚Ìƒwƒbƒ_[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

class CObject3D:public CObject 
{
public:
	CObject3D(int nPriority = PRIORITY_DEFAULT);
	~CObject3D()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void UpdateMatrix();

	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3& GetPos();
	void SetSize(D3DXVECTOR3 size);
	D3DXVECTOR3& GetSize();
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 &GetRot() { return m_rot; }
	void SetLength(D3DXVECTOR3 sizeA, D3DXVECTOR3 sizeB);
	float GetLength() { return m_length; }
	void SetAngle(float sizeA, float sizeB);
	float GetAngle() { return m_Angle; }

	const D3DXMATRIX& GetMatrix() const { return m_mtxWorld; }

	static CObject3D* Create(D3DXVECTOR3 pos);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pVtxTexture;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;
	float m_length;	// ”ÍˆÍ	
					//	|[>‰~‚Ì”»’è‚ÉŽg‚¤•¨
	float m_Angle;	// Šp“x
	D3DXMATRIX m_mtxWorld;
};

#endif // !_OBJECT3D_H_
