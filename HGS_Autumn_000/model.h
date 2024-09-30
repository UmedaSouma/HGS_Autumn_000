//===========================================================================================================================================================
// 
// model.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "object3D.h"
#include "modeldata.h"

class CModel :public CObject3D
{
private:
	CModeldata::ModelData m_ModelInfo;
public:
	// 向いている方向
	typedef enum
	{
		DIRECTION_NONE = 0,
		DIRECTION_LEFT,
		DIRECTION_RIGHT,
		DIRECTION_MAX
	}DIRECTION;

	CModel(int nPriority = PRIORITY_DEFAULT);
	~CModel();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void BindModel(CModeldata::ModelData* modelinfo);	// モデルの設定
	void SetDirection(DIRECTION dire);	// 向きの設定
	int GetDirection();					// 向きの取得

	void SetMat(D3DCOLORVALUE mat);
	void bMat(bool bmat) { m_bMat = bmat; }
	void SetDefaultMat(D3DCOLORVALUE mat, int nummat);
	void FixRot();	// 角度を修正する

	static CModel* Create(D3DXVECTOR3 pos);

	void SetModel(const char* address);
	void SetModelIdx(int nIdx) { m_nIdx = nIdx; }	// モデルの番号を保存する
	int GetModelIdx() { return m_nIdx; }	// モデル番号を取得する
	void SetModelAddress(const char* Address) { m_pModelAddress = Address; }
	const char* GetModelAddress() { return m_pModelAddress; }
private:
	int m_nIdx;
	int m_Direction;
	LPDIRECT3DTEXTURE9 m_apTexture[16] = {};
	D3DCOLORVALUE m_Diffuse[64];
	bool m_bMat;	// ダメージ状態のマテリアル変更
	const char* m_pModelAddress;	// モデルデータのアドレス
	
};

#endif // !_MODEL_H_
