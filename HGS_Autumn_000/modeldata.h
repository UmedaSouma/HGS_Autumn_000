#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include "main.h"

//====================================================================================
// モデルに必要な情報の構造体
//====================================================================================

// モデルデータのクラス
class CModeldata
{
public:
	typedef struct
	{
		LPD3DXMESH Mesh;			// メッシュ(頂点情報)へのポインタ
		LPD3DXBUFFER BuffMat;	// マテリアルへのポインタ
		DWORD NumMat;	// マテリアルの数
	}ModelData;


	static const unsigned int MAX_MODEL_DATA = 64;	// モデルデータの最大値
	static const unsigned int MAX_MAT_DATA = 514;	// １キャラクター分のマテリアルデータ最大値

	typedef struct
	{
		LPDIRECT3DTEXTURE9 pTex[MAX_MAT_DATA];
		int nIdx;
	}TexData;

	CModeldata();	// コンストラクタ
	~CModeldata();	// デストラクタ
	void Unload();	// 
	int Regist(const char* pTexturename);
	ModelData *GetAddress(int Idx);
	D3DCOLORVALUE *GetMatData(const char* pAddress, int MatNum);
private:
	
	TexData m_pTexData[MAX_MODEL_DATA];
	ModelData m_pModelData[MAX_MODEL_DATA];
	D3DCOLORVALUE m_StandardCol[MAX_MODEL_DATA][MAX_MAT_DATA];	// 色を変える前の元の色

	const char* m_pStorage[MAX_MODEL_DATA];
	static int m_NumAll;
	

	//LPD3DXMESH Mesh;			// メッシュ(頂点情報)へのポインタ
	//LPD3DXBUFFER BuffMat;	// マテリアルへのポインタ
	//DWORD NumMat;	// マテリアルの数
};

#endif // !_MODELDATA_H_
