//===========================================================================================================================================================
// 
// モデルの管理
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "model.h"
#include "manager.h"

//===================================================================================
// コンストラクタ
//===================================================================================
CModel::CModel(int nPriority) :CObject3D(nPriority)
, m_Direction(0)
, m_nIdx(-1)
, m_bMat(false)
, m_Diffuse{}
, m_pModelAddress{}
, m_apTexture{}
{
	m_ModelInfo = {};
}

//===================================================================================
// デストラクタ
//===================================================================================
CModel::~CModel()
{
}

//===================================================================================
// 初期設定
//===================================================================================
HRESULT CModel::Init()
{
	UpdateMatrix();

	return S_OK;
}

//===================================================================================
// 終了処理
//===================================================================================
void CModel::Uninit()
{
	Release();
}

//===================================================================================
// 更新処理
//===================================================================================
void CModel::Update()
{
	// オブジェクト3Dの更新
	CObject3D::Update();	// ここでマトリックスの設定をしている

	FixRot();
}

//===================================================================================
// 描画処理
//===================================================================================
void CModel::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルデータのポインタ

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &GetMatrix());

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (m_ModelInfo.BuffMat != nullptr
		&& m_ModelInfo.Mesh != nullptr
		&& m_ModelInfo.NumMat != 0)
	{
		// マテリアルデータのポインタの取得
		pMat = (D3DXMATERIAL*)m_ModelInfo.BuffMat->GetBufferPointer();
		CModeldata* pModeldata = CManager::GetModeldata();	// Modeldata のポインタを持ってくる

		if (pModeldata)
		{
			for (int nCntMat = 0; nCntMat < (int)m_ModelInfo.NumMat; nCntMat++)
			{
				if (m_bMat)
				{// ダメージを受けたとき
					pMat[nCntMat].MatD3D.Diffuse = { 1.0f,0.0f,0.0f,0.5f };	// 赤くする
				}
				else
				{// そのほかの状態の時

					//if (CScene::GetMode() != CScene::MODE_EDITOR)
					{
						pMat[nCntMat].MatD3D.Diffuse = *pModeldata->GetMatData(m_pModelAddress, nCntMat);	// 通常色にする
					}

				}

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, nullptr);

				// モデルパーツの描画
				m_ModelInfo.Mesh->DrawSubset(nCntMat);
			}
		}
	
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===========================================================================================================
// 角度を修正する
//===========================================================================================================
void CModel::FixRot()
{
	if (GetRot().x >= D3DX_PI*2 ||  GetRot().x <= -D3DX_PI*2)
	{
		SetRot({ 0.0f,GetRot().y,GetRot().z });
	}
	if (GetRot().y >= D3DX_PI * 2 || GetRot().y <= -D3DX_PI * 2)
	{
		SetRot({ GetRot().x,0.0f,GetRot().z });
	}
	if (GetRot().z >= D3DX_PI * 2 ||  GetRot().z <= -D3DX_PI * 2)
	{
		SetRot({ GetRot().x,GetRot().y,0.0f });
	}
}

//===================================================================================
// 生成処理
//===================================================================================
CModel* CModel::Create(D3DXVECTOR3 pos)
{
	CModel* pModel = new CModel;

	pModel->SetPos(pos);
	pModel->SetType(TYPE::MODEL);

	pModel->Init();

	return pModel;
}

//===================================================================================
// モデルの情報をセットする
//===================================================================================
void CModel::BindModel(CModeldata::ModelData *modelinfo)
{
	m_ModelInfo.Mesh = modelinfo->Mesh;
	m_ModelInfo.BuffMat = modelinfo->BuffMat;
	m_ModelInfo.NumMat = modelinfo->NumMat;
}

//===========================================================================================================
// 向きを設定
//===========================================================================================================
void CModel::SetDirection(DIRECTION dire)
{
	switch (dire)
	{
	case DIRECTION_LEFT:
		m_Direction = -1;
		SetRot({
			0.0f
			,D3DX_PI * 0.5f
			,0.0f
			});
		break;

	case DIRECTION_RIGHT:
		m_Direction = 1;
		SetRot({
			0.0f
			,D3DX_PI * -0.5f
			,0.0f
			});
		break;

	default:
		break;
	}
}

//===========================================================================================================
// 向きを取得
//===========================================================================================================
int CModel::GetDirection()
{
	return m_Direction;
}

//===========================================================================================================
// モデルの見た目設定
//===========================================================================================================
void CModel::SetModel(const char* address)
{
	CModeldata* pModeldata = nullptr;
	int nIdx = 0;

	SetModelAddress(address);		// アドレスを保存しておく
	pModeldata = CManager::GetModeldata();
	nIdx = pModeldata->Regist(address);
	BindModel(CManager::GetModeldata()->GetAddress(nIdx));
}