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
CObjectX::CObjectX(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ModelInfo.m_pTexture = nullptr;
}

//===================================================================================
// デストラクタ
//===================================================================================
CObjectX::~CObjectX()
{
	if (m_ModelInfo.m_pTexture != nullptr)
	{
		// テクスチャの破棄
		for (DWORD i = 0; i < m_ModelInfo.NumMat; i++)
		{
			if (m_ModelInfo.m_pTexture[i] != nullptr)
			{
				m_ModelInfo.m_pTexture[i]->Release();
			}
		}
		delete[]m_ModelInfo.m_pTexture;
		m_ModelInfo.m_pTexture = nullptr;
	}
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
			pDevice->SetTexture(0, m_ModelInfo.m_pTexture[nCntMat]);

			// モデルパーツの描画
			m_ModelInfo.Mesh->DrawSubset(nCntMat);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
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
// モデルの情報をセットする
//===================================================================================
void CObjectX::SetModelInfo(const char aPath[FILE_PATH])
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = CManager::GetRenderer()->GetDevice();

	strcpy(&m_cFileName[0], &aPath[0]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(m_cFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_ModelInfo.BuffMat,
		NULL,
		&m_ModelInfo.NumMat,
		&m_ModelInfo.Mesh);

	// マテリアル情報の取得
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_ModelInfo.BuffMat->GetBufferPointer();

	m_pTexture = new LPDIRECT3DTEXTURE9[m_ModelInfo.NumMat];

	for (DWORD i = 0; i < m_ModelInfo.NumMat; i++)
	{
		m_pTexture[i] = nullptr;

		if (pMat[i].pTextureFilename)
		{
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[i].pTextureFilename, &m_pTexture[i]);
		}
	}

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