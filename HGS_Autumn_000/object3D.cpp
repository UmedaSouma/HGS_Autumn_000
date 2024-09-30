#include "manager.h"
#include "object3D.h"
#include "texture.h"

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
, m_pos{ 0.0f, 0.0f, 0.0f }
, m_rot{ 0.0f,0.0f,0.0f }
, m_size{ 0.0f,0.0f,0.0f }
, m_pVtxBuff(nullptr)
, m_pVtxTexture(nullptr)
, m_length(0.0f)
, m_Angle(0.0f)
{
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
CObject3D::~CObject3D()
{
}

//===========================================================================================================
// 初期設定
//===========================================================================================================
HRESULT CObject3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	// 座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, 0.0f, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, 0.0f, -m_size.z);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// カラーの設定
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{// 白
		pVtx[nCnt].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_rot.x = (D3DX_PI * -0.5f);

	m_pVtxBuff->Unlock();

	// 生成した瞬間に描画がおかしくならないように Matrix の計算を行う
	UpdateMatrix();

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void CObject3D::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//// テクスチャの破棄
	//if (m_pVtxTexture != nullptr)
	//{
	//	m_pVtxTexture->Release();
	//	m_pVtxTexture = nullptr;
	//}

	Release();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void CObject3D::Update()
{
	UpdateMatrix();
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void CObject3D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//if (pPlayer3D == nullptr)
	{
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点描画
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_pVtxTexture);

		// ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//===========================================================================================================
// 生成処理
//===========================================================================================================
CObject3D* CObject3D::Create(D3DXVECTOR3 pos)
{
	CObject3D* pObject3D = nullptr;
	pObject3D = new CObject3D;

	pObject3D->m_pos = pos;

	pObject3D->Init();

	return pObject3D;
}

//===========================================================================================================
// マトリックスの更新
//===========================================================================================================
void CObject3D::UpdateMatrix()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;

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
}

//===========================================================================================================
// 位置を設定
//===========================================================================================================
void CObject3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================================================================================================
// 位置を取得
//===========================================================================================================
D3DXVECTOR3& CObject3D::GetPos()
{
	return m_pos;
}

//===========================================================================================================
// サイズを設定
//===========================================================================================================
void CObject3D::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//===========================================================================================================
// サイズを取得
//===========================================================================================================
D3DXVECTOR3& CObject3D::GetSize()
{
	return m_size;
}

//========================================================================================================================
// 任意のテクスチャを設定する
//========================================================================================================================
void CObject3D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pVtxTexture = pTex;
}

void CObject3D::SetLength(D3DXVECTOR3 sizeA, D3DXVECTOR3 sizeB)
{
}
