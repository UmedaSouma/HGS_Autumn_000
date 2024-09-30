//===========================================================================================================================================================
// 
// camera.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
//===========================================================================================================================================================
// 
// camera.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

class CCamera
{
private:
	const float ROT_SPEED = 0.1f;

public:
	CCamera();
	~CCamera();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetCamera();	// カメラの設定
	void SetTargetPos(D3DXVECTOR3 pos);
	void SetShake(int frame, float strength);	// カメラ揺れの処理
	
private:
	D3DXVECTOR3 m_posV;
	D3DXVECTOR3 m_posR;
	D3DXVECTOR3 m_targetpos;
	D3DXVECTOR3 m_vecU;
	D3DXVECTOR3 m_rot;
	D3DXMATRIX m_mtxProjection;
	D3DXMATRIX m_mtxView;
	float m_fDis;
	float m_follow=0.001f;
	int nCntFollow;
	int m_nFrame;	// カメラ揺れのフレーム数
	float m_fStrength;	// カメラ揺れの強度
public:
	D3DXVECTOR3 GetPos() { return m_posV; }
};



//// カメラの構造体
//typedef struct
//{
//	D3DXVECTOR3 posV;			// 視点
//	D3DXVECTOR3 posR;			// 注視点
//	D3DXVECTOR3 vecU;			// 上方向ベクトル
//	D3DXVECTOR3 rot;
//	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
//	D3DXMATRIX mtxView;			// ビューマトリックス
//	D3DXVECTOR3 targetPosV;
//	D3DXVECTOR3 targetPosR;
//	D3DXVECTOR3 fDis;
//}Camera;
//
//// プロトタイプ宣言
//void InitCamera(void);
//void UninitCamera(void);
//void UpdateCamera(void);
//void SetCamera(void);
//Camera* GetCamera(void);
//void SetPosCamera(D3DXVECTOR3 pos);

#endif