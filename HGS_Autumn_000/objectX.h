//===========================================================================================================================================================
// 
// model.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _OBJEXTX_H_
#define _OBJEXTX_H_

#include "main.h"
#include "object.h"

class CObjectX :public CObject
{
private:
	typedef struct
	{
		LPD3DXMESH Mesh;			// メッシュ(頂点情報)へのポインタ
		LPD3DXBUFFER BuffMat;	// マテリアルへのポインタ
		DWORD NumMat;	// マテリアルの数
		LPDIRECT3DTEXTURE9* m_pTexture;	// テクスチャのポインタ
	}ModelInfo;

	ModelInfo m_ModelInfo;
public:
	CObjectX(int nPriority = 3);
	~CObjectX();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3& GetPos();
	
	const static int FILE_PATH = 128;											// ファイルパスの最大文字数

	void SetModelInfo(const char aPath[FILE_PATH]);

	static CObjectX* Create(D3DXVECTOR3 pos);


private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_scale;
	D3DXMATRIX m_mtxWorld;

	char m_cFileName[FILE_PATH];												// Xファイルのパス
	LPDIRECT3DTEXTURE9* m_pTexture;												// テクスチャの情報

};

#endif // !_MODEL_H_
