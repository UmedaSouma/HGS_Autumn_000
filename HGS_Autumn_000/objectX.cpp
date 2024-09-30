//===========================================================================================================================================================
// 
// モデルの管理
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "objectX.h"
#include "player3D.h"
#include "manager.h"

//===================================================================================
// コンストラクタ
//===================================================================================
CObjectX::CObjectX():m_pos{ 0.0f,0.0f,0.0f }
,m_rot{ 0.0f,0.0f,0.0f }
,m_scale{ 0.0f,0.0f,0.0f }
{
}

//===================================================================================
// デストラクタ
//===================================================================================
CObjectX::~CObjectX()
{
}

//===================================================================================
// 初期設定
//===================================================================================
HRESULT CObjectX::Init()
{
	return S_OK;
}

//===================================================================================
// 終了処理
//===================================================================================
void CObjectX::Uninit()
{
	CPlayer3D* pPlayer3D = dynamic_cast<CPlayer3D*>(this);
	if (pPlayer3D == nullptr)
	{
		//if (m_ModelInfo.Mesh != nullptr)
		//{
		//	m_ModelInfo.Mesh->Release();
		//	m_ModelInfo.Mesh = nullptr;
		//}
	}

	//if (m_ModelInfo.BuffMat != nullptr)
	//{
	//	m_ModelInfo.BuffMat->Release();
	//	m_ModelInfo.BuffMat = nullptr;
	//}

	Release();
}

//===================================================================================
// 更新処理
//===================================================================================
void CObjectX::Update()
{
}

//===================================================================================
// 描画処理
//===================================================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルデータのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (m_ModelInfo.BuffMat != nullptr
		&& m_ModelInfo.Mesh != nullptr
		&& m_ModelInfo.NumMat != 0)
	{
		// マテリアルデータのポインタの取得
		pMat = (D3DXMATERIAL*)m_ModelInfo.BuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_ModelInfo.NumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, nullptr);

			// モデルパーツの描画
			m_ModelInfo.Mesh->DrawSubset(nCntMat);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===================================================================================
// モデルの情報をセットする
//===================================================================================
void CObjectX::SetModelInfo(LPD3DXMESH mesh, LPD3DXBUFFER buffmat, DWORD nummat)
{
	// 各変数の初期化
	m_ModelInfo.Mesh = nullptr;			// メッシュ(頂点情報)へのポインタ
	m_ModelInfo.BuffMat = nullptr;	// マテリアルへのポインタ
	m_ModelInfo.NumMat = 0;	// マテリアルの数

	m_ModelInfo.Mesh = mesh;
	m_ModelInfo.BuffMat = buffmat;
	m_ModelInfo.NumMat = nummat;
}

//===================================================================================
// 位置を設定
//===================================================================================
void CObjectX::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===================================================================================
// 位置を取得
//===================================================================================
D3DXVECTOR3& CObjectX::GetPos()
{
	return m_pos;
}

//===================================================================================
// 生成処理
//===================================================================================
CObjectX* CObjectX::Create(D3DXVECTOR3 pos)
{
	CObjectX* pModel = new CObjectX;

	pModel->SetPos(pos);
	pModel->SetType(TYPE::MODEL);

	pModel->Init();

	return pModel;
}