//===========================================================================================================================================================
// 
// オブジェクトの頂点座標の管理
// Author : souma umeda
// 
//===========================================================================================================================================================

// インクルード
#include "object2D.h"
#include "renderer.h"
#include "manager.h"


//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CObject2D::CObject2D(int nPriority) :CObject(nPriority)
, m_nTexSplit{ 1,1 }
, m_color{ 1.0f,1.0f ,1.0f ,1.0f }
, m_fAngle(0)
, m_fLength(0)
, m_size{ 1.0f,1.0f ,1.0f }
, m_rot{ 0.0f,0.0f ,0.0f }
, m_pos{ 0.0f,0.0f ,0.0f }
, m_nCnt(0)
, m_pVtxBuff(nullptr)
, m_pVtxTexture(nullptr)
, m_nAnimeCounter{ 0,0 }
, m_nAnimeInterval(0)
, m_bHP(false)
, m_fHPratio(1.0f)
{
	m_nPriority = nPriority;
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CObject2D::~CObject2D()
{

}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CObject2D::Init()
{
	CRenderer* Renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	// 頂点バッファの設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, &TexFile[m_texfile][0], &m_pVtxTexture);

	//m_pVtxTexture = nullptr;

	// 頂点バッファのポインタ生成
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	// 対角線
	m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) / 2.0f;

	// 角度
	m_fAngle = atan2f(m_size.x, m_size.y);
	
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// 座標の設定
	if (m_bHP)
	{
		pVtx[0].pos.x = m_pos.x;
		pVtx[0].pos.y = m_pos.y - m_size.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + (m_size.x * m_fHPratio);
		pVtx[1].pos.y = m_pos.y - m_size.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x;
		pVtx[2].pos.y = m_pos.y + m_size.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + m_size.x * m_fHPratio;
		pVtx[3].pos.y = m_pos.y + m_size.y;
		pVtx[3].pos.z = 0.0f;
	}
	
	// 法線の設定
	{
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	}
	
	// カラーの設定
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	// テクスチャ座標の設定
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / m_nTexSplit.x, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / m_nTexSplit.y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / m_nTexSplit.x, 1.0f / m_nTexSplit.y);
	}
	
	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CObject2D::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// テクスチャの破棄
	{
		// 弾テクスチャ以外を破棄
		CFade* pFade = dynamic_cast<CFade*>(this);	// castしたとき型が一致していないとnullが入る

		if (pFade != nullptr)
		{
			// テクスチャの破棄
			if (m_pVtxTexture != nullptr)
			{
				m_pVtxTexture->Release();
				m_pVtxTexture = nullptr;
			}
		}
	}

	Release();
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CObject2D::Update()
{
	//CRenderer* Renderer = CManager::GetRenderer();
	//LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

	// 頂点ポインタ生成
	VERTEX_2D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// 座標の設定
	if (m_bHP)
	{
		pVtx[0].pos.x = m_pos.x;
		pVtx[0].pos.y = m_pos.y - m_size.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + (m_size.x * m_fHPratio);
		pVtx[1].pos.y = m_pos.y - m_size.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x;
		pVtx[2].pos.y = m_pos.y + m_size.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + (m_size.x * m_fHPratio);
		pVtx[3].pos.y = m_pos.y + m_size.y;
		pVtx[3].pos.z = 0.0f;
	}

	{//// 座標の設定
	//pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[0].pos.z = 0.0f;

	//pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
	//pVtx[1].pos.z = 0.0f;

	//pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	//pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	//pVtx[2].pos.z = 0.0f;

	//pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	//pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	//pVtx[3].pos.z = 0.0f;

	}

	pVtx[0].col = m_color;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(1.0f / m_nTexSplit.x * m_nAnimeCounter.x, 1.0f / m_nTexSplit.y * m_nAnimeCounter.y);
	pVtx[1].tex = D3DXVECTOR2(1.0f / m_nTexSplit.x * (m_nAnimeCounter.x + 1), 1.0f / m_nTexSplit.y * m_nAnimeCounter.y);
	pVtx[2].tex = D3DXVECTOR2(1.0f / m_nTexSplit.x * m_nAnimeCounter.x, 1.0f / m_nTexSplit.y * (m_nAnimeCounter.y + 1));
	pVtx[3].tex = D3DXVECTOR2(1.0f / m_nTexSplit.x * (m_nAnimeCounter.x + 1), 1.0f / m_nTexSplit.y * (m_nAnimeCounter.y + 1));

		//// カウントがインターバルフレームまでカウントしたら
		//if (m_nCnt == m_nAnimeInterval)
		//{
		//	// 横の分割数に到達するまで
		//	if (m_nAnimeCounter.x < m_nTexSplit.x)
		//	{
		//		m_nAnimeCounter.x++;	// 横アニメーション進める
		//	}

		//	// 横のアニメーションカウンターが横の分割数超えたら
		//	if (m_nAnimeCounter.x >= m_nTexSplit.x)
		//	{
		//		m_nAnimeCounter.y++;	// 縦アニメーションを進める
		//		m_nAnimeCounter.x = 0;	// 横のアニメーションをリセットする
		//	}

		//	// 縦のアニメーションカウンターが縦の分割数超えたら
		//	if (m_nAnimeCounter.y >= m_nTexSplit.y)
		//	{
		//		m_nAnimeCounter.y = 0;	// 縦のアニメーションをリセットする
		//	}		

		//	// アニメーションカウンターリセット
		//	m_nCnt = 0;
		//}

		// アニメーションカウンターを進める
		m_nCnt++;

		// 頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	
}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CObject2D::Draw()
{
	// レンダラーを取得
	CRenderer* Renderer = CManager::GetRenderer();

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();


	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点描画
	pDevice->SetFVF(FVF_VERTEX_2D);


	if (m_pVtxTexture == nullptr)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, nullptr);
	}
	else if (m_pVtxTexture != nullptr)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, m_pVtxTexture);
	}
	

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================================================================================================
// 任意のテクスチャを設定する
//========================================================================================================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pVtxTexture = pTex;
}

//========================================================================================================================
// オブジェクト2Dの生成
//========================================================================================================================
CObject2D* CObject2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR2 TexSplit, int Interval)
{
	CObject2D* Object2D = new CObject2D;

	Object2D->m_pos = pos;					// 位置の設定
	Object2D->m_size = size;				// サイズの設定
	Object2D->m_nTexSplit = TexSplit;		// テクスチャ分割数の設定
	Object2D->m_nAnimeInterval = Interval;	// テクスチャアニメーションの間隔を設定
	Object2D->m_nAnimeCounter.x = {};		// 何枚目のアニメ―ソヨンが動いているか確認するカウンター

	Object2D->Init();	// 初期設定

	return Object2D;
}

//========================================================================================================================
// アニメーションカウンターの取得
//========================================================================================================================
D3DXVECTOR2& CObject2D::GetAnimeCounter()
{
	return m_nAnimeCounter;
}

//========================================================================================================================
// アニメーションの間隔の設定
//========================================================================================================================
void CObject2D::SetAnimeInterval(int Interval)
{
	m_nAnimeInterval = Interval;
}

//========================================================================================================================
// 描画優先順位の取得
//========================================================================================================================
int CObject2D::GetPriority()
{
	return m_nPriority;
}