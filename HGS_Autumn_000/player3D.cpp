//===========================================================================================================================================================
// 
// 3Dプレイヤーの処理
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "player3D.h"
#include "manager.h"
#include "game.h"
#include "search.h"
#include "block3D.h"

// 静的メンバ変数の初期化
int CPlayer3D::m_nFragmentNum = 0;

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CPlayer3D::CPlayer3D(int nPriority) :CObjectX(nPriority)
, m_Move{ 0.0f,0.0f,0.0f }
, m_fSpeed(0)
, m_oldPos{ 0.0f,0.0f,0.0f }
, m_skillstate(SKILL_NONE)
, m_bUseJump(false)
, m_bSlip(false)
, m_nSlipCnt(0)
, m_bDammage(false)
, m_nLife(0)
, m_nCntDamage(0)
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CPlayer3D::~CPlayer3D()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CPlayer3D::Init()
{
	// モデルの設定
	CObjectX::SetModelInfo("data\\model\\BallPlayer.x");
	//SetModelAddress("data\\model\\BallPlayer.x");		// アドレスを保存しておく
	//CModeldata* pModeldata = CManager::GetModeldata();	// modeldata のポインタを持ってくる
	//int nIdx = pModeldata->Regist(GetModelAddress());	// モデルデータの登録
	//BindModel(pModeldata->GetAddress(nIdx));			// モデル情報をセットする
	//SetModelIdx(nIdx);
	//SetTexture(pModeldata->GetTexture(nIdx));

	CObjectX::Init();

	//SetSize({ 10.0f, 20.0f, 15.0f });
	m_fSpeed = 1.0f;

	m_nLife = m_MaxLife;

	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CPlayer3D::Uninit()
{
	CObjectX::Uninit();
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CPlayer3D::Update()
{
	D3DXVECTOR3 pos = GetPos();

	// 当たり判定消す(後々関数化
	{
		if (m_nSlipCnt == 0)
		{
			m_bSlip = false;
		}
		if (m_bSlip)
		{
			m_nSlipCnt--;
		}

		DeathPlayer();
	}

	// プレイヤー操作
	InputPosPlayer();

	if (!m_bSlip)
	{
		m_bUseJump = true;

		// Y軸の当たり判定・更新
		pos.y += m_Move.y;
		SetPos(pos);
		//pos = LookObjectCollision(COLLISION_HIGHLOW);

		// X軸の当たり判定・更新
		pos.x += m_Move.x;
		SetPos(pos);
		//pos = LookObjectCollision(COLLISION_HORIZON);

		// Z軸の当たり判定・更新
		pos.z += m_Move.z;
		SetPos(pos);
		//pos = LookObjectCollision(COLLISION_VERTICAL);
	}

	// カメラ追尾
	SetTargetCamera();

	// 速度徐々に減衰
	m_Move.x += (0.0f - m_Move.x) * 0.5f;
	m_Move.z += (0.0f - m_Move.z) * 0.5f;
	//m_Move.y += (0.0f - m_Move.y) * 0.2f;

	CObjectX::Update();

	if (m_bDammage)
	{
		UpdateDamage();
	}

	// 過去の位置を登録
	m_oldPos = pos;
}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CPlayer3D::Draw()
{
	CObjectX::Draw();
}

//========================================================================================================================
// プレイヤーの入力処理
//========================================================================================================================
D3DXVECTOR3 CPlayer3D::InputPosPlayer()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	if (keyboard->GetPress(DIK_A) || joypad->GetPress(CInputJoypad::JOYKEY_LEFT))
	{
		m_Move.x += sinf(-D3DX_PI * 0.5f) * m_fSpeed;
		m_Move.y += cosf(-D3DX_PI * 0.5f) * m_fSpeed;

		//SetDirection(CModel::DIRECTION_LEFT);
	}
	if (keyboard->GetPress(DIK_D)||joypad->GetPress(CInputJoypad::JOYKEY_RIGHT))
	{
		m_Move.x += sinf(-D3DX_PI * 0.5f) * -m_fSpeed;
		m_Move.y += cosf(-D3DX_PI * 0.5f) * -m_fSpeed;

		//SetDirection(CModel::DIRECTION_RIGHT);
	}
	//if (keyboard->GetPress(DIK_W))
	//{
	//	m_Move.z += sinf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//	m_Move.x += cosf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//}
	//if (keyboard->GetPress(DIK_S))
	//{
	//	m_Move.z -= sinf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//	m_Move.x -= cosf(-D3DX_PI * 0.5f) * -m_fSpeed;
	//}

	// ジャンプ重力処理
	Jump();

	//// 画面を揺らす処理
	//if (keyboard->GetTrigger(DIK_F1))
	//{
	//	CManager::GetCamera()->SetShake(120, 5.0f);
	//}

	return m_Move;
}

//========================================================================================================================
// プレイヤーの入力処理
//========================================================================================================================
void CPlayer3D::Jump()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CInputJoypad* joypad = CManager::GetJoypad();

	if (!m_bUseJump)
	{
		if (keyboard->GetTrigger(DIK_SPACE) || joypad->GetTrigger(CInputJoypad::JOYKEY_A))
		{
			m_Move.y += m_fJumpPower;	// ジャンプ
			m_bUseJump = true;
		}
	}

	m_Move.y -= m_fGravity;	// 重力加算
}

//===========================================================================================================
// ダメージ増やす処理
//===========================================================================================================
void CPlayer3D::AddDamage(int nDamage)
{
	if (!m_bDammage)
	{// ダメージ状態ではないとき

		m_nCntDamage = m_MaxInterval;	// ダメージ状態のカウントを設定
		m_nLife -= nDamage;	// ダメージ分ライフを減らす
		m_bDammage = true;	// ダメージ状態にする
	}
}

//===========================================================================================================
// ダメージ状態の更新
//===========================================================================================================
void CPlayer3D::UpdateDamage()
{
	m_nCntDamage--;	// ダメージ状態のカウントを減らす
	//bMat(true);

	if (m_nCntDamage <= 0)
	{
		//bMat(false);
		m_bDammage = false;
	}
}

//===========================================================================================================
// 死んだときの処理
//===========================================================================================================
void CPlayer3D::DeathPlayer()
{
	if (GetPos().y <= -50.0f || m_nLife <= 0)
	{// 穴に落ちたとき || 体力がなくなったとき
		m_nLife = m_MaxLife;			// 体力を最大値に戻す
		SetPos({ 40.0f, 25.0f, 0.0f });	// リスポーンさせる
		m_bSlip = true;					// すり抜けさせる
		m_nSlipCnt = 5;					// すり抜けさせる時間を設定
	}
}

//========================================================================================================================
// 生成処理
//========================================================================================================================
CPlayer3D* CPlayer3D::Create(D3DXVECTOR3 pos)
{
	CPlayer3D* pPlayer3D = new CPlayer3D;	// プレイヤーの型で動的確保

	pPlayer3D->SetPos(pos);	// 位置を設定
	pPlayer3D->SetType(TYPE::PLAYER);

	pPlayer3D->Init();	// ３Dの初期設定

	return pPlayer3D;
}

//========================================================================================================================
// playerのposをcameraのtargetにセットする
//========================================================================================================================
void CPlayer3D::SetTargetCamera()
{
	CCamera* pCamera = CManager::GetCamera();	// camera を持ってくる
	pCamera->SetTargetPos(GetPos());			// camera に playerpos を入れる
}

////========================================================================================================================
//// 各オブジェクトを確認する
////========================================================================================================================
//D3DXVECTOR3 CPlayer3D::LookObjectCollision(COLLISION_TYPE type)
//{
//	D3DXVECTOR3 pos = {};
//
//	for (int nPrio = 0; nPrio < CObject::PRIORITY_MAX; nPrio++)
//	{
//		CObject* pObj = CObject::GetTop(nPrio);
//
//		while (pObj != nullptr)
//		{
//			CObject* pNext = pObj->GetNext();
//
//			switch (type)
//			{
//				// 縦の当たり判定(z軸)
//			case COLLISION_VERTICAL:
//				pos = VerticalCollision(pObj);
//				break;
//
//				// 横の当たり判定(x軸)
//			case COLLISION_HORIZON:
//				pos = HorizonCollision(pObj);
//				break;
//
//				// 上下の当たり判定(y軸)
//			case COLLISION_HIGHLOW:
//				pos = HighLowCollision(pObj);
//				break;
//
//			default:
//				assert(1);
//				break;
//			}
//
//			pObj = pNext;
//		}
//	}
//
//	return pos;
//}
//
////========================================================================================================================
//// 縦の当たり判定
////========================================================================================================================
//D3DXVECTOR3 CPlayer3D::VerticalCollision(CObject* pObj)
//{
//	//CObject::TYPE type = pObj->GetType();
//
//	//D3DXVECTOR3 PlayerPos = GetPos();
//	//D3DXVECTOR3 PlayerSize = GetSize();
//
//	//PlayerSize.x *= 0.5f;
//	//PlayerSize.y *= 0.5f;
//	//PlayerSize.z *= 0.5f;
//
//	//if (type == TYPE::BLOCK)
//	//{
//	//	CBlock3D* pBlock = (CBlock3D*)pObj;
//
//	//	D3DXVECTOR3 BlockPos = pBlock->GetPos();
//	//	D3DXVECTOR3 BlockSize = pBlock->GetSize();
//
//	//	BlockSize.x *= 0.5f;
//	//	BlockSize.y *= 0.5f;
//	//	BlockSize.z *= 0.5f;
//
//	//	if (BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z		// プレイヤーがブロックにめり込んだ時
//	//		&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
//	//		&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// プレイヤーがブロックの
//	//		&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// xy範囲内にいたとき
//	//		&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// -------------------------------------------
//	//		&& m_oldPos.z + PlayerSize.z <= BlockPos.z - BlockSize.z	// プレイヤーの過去の位置がブロックより手前にあったとき
//	//		)
//	//	{// 上に乗る
//	//		m_Move.z = 0;
//
//	//		SetPos({ PlayerPos.x,PlayerPos.y,PlayerPos.z = (BlockPos.z - BlockSize.z - PlayerSize.z) });
//	//	}
//
//	//	if (BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z		// プレイヤーがブロックにめり込んだ時
//	//		&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
//	//		&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// プレイヤーがブロックの
//	//		&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// xy範囲内にいたとき
//	//		&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// -------------------------------------------
//	//		&& m_oldPos.z - PlayerSize.z >= BlockPos.z + BlockSize.z	// プレイヤーの過去の位置がブロックより奥にあったとき
//	//		)
//	//	{
//	//		m_Move.z = 0;
//
//	//		SetPos({ PlayerPos.x,PlayerPos.y ,PlayerPos.z = (BlockPos.z + BlockSize.z + PlayerSize.z) });
//	//	}
//	//}
//
//	//return PlayerPos;
//}
//
////========================================================================================================================
//// 横の当たり判定
////========================================================================================================================
//D3DXVECTOR3 CPlayer3D::HorizonCollision(CObject* pObj)
//{
//	//CObject::TYPE type = pObj->GetType();
//
//	//D3DXVECTOR3 PlayerPos = GetPos();
//	////D3DXVECTOR3 PlayerSize = GetSize();
//
//	//PlayerSize.x *= 0.5f;
//	//PlayerSize.y *= 0.5f;
//	//PlayerSize.z *= 0.5f;
//
//	//if (type == TYPE::BLOCK)
//	//{
//	//	CBlock3D* pBlock = (CBlock3D*)pObj;
//
//	//	D3DXVECTOR3 BlockPos = pBlock->GetPos();
//	//	D3DXVECTOR3 BlockSize = pBlock->GetSize();
//
//	//	BlockSize.x *= 0.5f;
//	//	BlockSize.y *= 0.5f;
//	//	BlockSize.z *= 0.5f;
//
//	//	if (BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x		// プレイヤーがブロックにめり込んだ時
//	//		&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// -------------------------------------------
//	//		&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// プレイヤーがブロックの
//	//		&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// yz範囲内にいたとき
//	//		&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
//	//		&& m_oldPos.x - PlayerSize.x >= BlockPos.x + BlockSize.x	// プレイヤーの過去の位置がブロックより右にあったとき
//	//		)
//	//	{
//	//		m_Move.x = 0;
//
//	//		SetPos({ PlayerPos.x = (BlockPos.x + BlockSize.x + PlayerSize.x),PlayerPos.y,PlayerPos.z });
//	//	}
//
//	//	else if (BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// プレイヤーがブロックにめり込んだ時
//	//		&& BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y	// -------------------------------------------
//	//		&& BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// プレイヤーがブロックの
//	//		&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// yz範囲内にいたとき
//	//		&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
//	//		&& m_oldPos.x + PlayerSize.x <= BlockPos.x - BlockSize.x	// プレイヤーの過去の位置がブロックより左にあったとき
//	//		)
//	//	{
//	//		m_Move.x = 0;
//
//	//		SetPos({ PlayerPos.x = (BlockPos.x - BlockSize.x - PlayerSize.x),PlayerPos.y,PlayerPos.z });
//	//	}
//	//}
//
//	//return PlayerPos;
//}
//
////========================================================================================================================
//// 横の当たり判定
////========================================================================================================================
//D3DXVECTOR3 CPlayer3D::HighLowCollision(CObject* pObj)
//{
//	CObject::TYPE type = pObj->GetType();
//
//	D3DXVECTOR3 PlayerPos = GetPos();
//	//D3DXVECTOR3 PlayerSize = GetSize();
//
//	PlayerSize.x *= 0.5f;
//	PlayerSize.z *= 0.5f;
//	PlayerSize.y *= 0.5f;
//
//	if (type == TYPE::BLOCK)
//	{// タイプがブロックだった場合
//
//		CBlock3D* pBlock = (CBlock3D*)pObj;
//
//		D3DXVECTOR3 BlockPos = pBlock->GetPos();
//		D3DXVECTOR3 BlockSize = pBlock->GetSize();
//
//		BlockSize.x *= 0.5f;
//		BlockSize.y *= 0.5f;
//		BlockSize.z *= 0.5f;
//
//		// プレイヤーがブロックに頭をぶつける
//		if (BlockPos.y - BlockSize.y < PlayerPos.y + PlayerSize.y		// プレイヤーがブロックにめり込んだ時
//			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
//			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// プレイヤーがブロックの
//			&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// xz範囲内にいたとき
//			&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
//			&& BlockPos.y - BlockSize.y >= m_oldPos.y + PlayerSize.y	// プレイヤーの過去の位置がブロックより下にあったとき
//			)
//		{
//			m_Move.y = 0;
//
//			SetPos({ PlayerPos.x,PlayerPos.y = (BlockPos.y - BlockSize.y - PlayerSize.y),PlayerPos.z });	// プレイヤーがブロックの下に行く
//		}
//
//		// プレイヤーがブロックに乗る
//		else if (BlockPos.y + BlockSize.y > PlayerPos.y - PlayerSize.y	// プレイヤーがブロックにめり込んだ時
//			&& BlockPos.x - BlockSize.x < PlayerPos.x + PlayerSize.x	// -------------------------------------------
//			&& BlockPos.x + BlockSize.x > PlayerPos.x - PlayerSize.x	// プレイヤーがブロックの
//			&& BlockPos.z - BlockSize.z < PlayerPos.z + PlayerSize.z	// xz範囲内にいたとき
//			&& BlockPos.z + BlockSize.z > PlayerPos.z - PlayerSize.z	// -------------------------------------------
//			&& BlockPos.y + BlockSize.y <= m_oldPos.y - PlayerSize.y	// プレイヤーの過去の位置がブロックより上にあったとき
//			)
//		{
//			m_bUseJump = false;
//			m_Move.y = 0;
//			SetPos({ PlayerPos.x,PlayerPos.y = (BlockPos.y + BlockSize.y + PlayerSize.y),PlayerPos.z });	// プレイヤーがブロックの上に乗る
//		}
//	}
//
//	return PlayerPos;
//}