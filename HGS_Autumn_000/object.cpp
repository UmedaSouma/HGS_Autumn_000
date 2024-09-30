#include "object.h"

//************************************************
// < debugmemo >
// for_each の残骸
//************************************************
//#include <algorithm>

// 静的メンバ変数の初期化
int CObject::m_nNumAll = 0;				// オブジェクト総数
CObject* CObject::m_apObject[PRIORITY_MAX][MAX_OBJECT] = {};	// オブジェクトの管理

CObject* CObject::m_pObjectList = nullptr;
CObject* CObject::m_pTop[PRIORITY_MAX] = { nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
CObject* CObject::m_pCur[PRIORITY_MAX] = { nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

CObject::CObject(int nPriority)
{// 自分自身の登録

	if (m_pTop[nPriority] == nullptr)
	{// top が設定されていなかったら
		m_pPrev = nullptr;	// 
		m_pNext = nullptr;	// 
		m_pTop[nPriority] = this;		// 最初のポインタとして設定
		m_pCur[nPriority] = this;		// 最後尾のポインタとして設定
	}
	else
	{
		m_pNext = nullptr;			// 次のポインタをヌルにしておく
		m_pPrev = m_pCur[nPriority];			// この時点での最後尾を前のポインタに設定する
		m_pCur[nPriority] = this;				// 最後尾を自身に設定する
		m_pPrev->m_pNext = this;	// 前のポインタの次のポインタに自身を設定する
	}

	m_nNumAll++;				// オブジェクトの総数を増やす
	m_nPriority = nPriority;
	m_type = TYPE::NONE;
	m_bRelese = false;


	//for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	//{
	//	if (m_apObject[nPriority][nCnt] == nullptr)
	//	{
	//		m_apObject[nPriority][nCnt] = this;	// 自分自身を代入
	//		m_nID = nCnt;				// 自分自身のIDを代入
	//		m_nNumAll++;				// オブジェクトの総数を増やす
	//		m_nPriority = nPriority;
	//		m_type = TYPE::NONE;
	//		m_bRelese = false;

	//		break;
	//	}
	//}
}

CObject::~CObject()
{

}

HRESULT CObject::Init()
{
	// 純粋仮想関数

	return S_OK;
}

void CObject::Uninit()
{
	// 純粋仮想関数
}

void CObject::Update()
{
	// 純粋仮想関数
}

void CObject::Draw()
{
	// 純粋仮想関数
}

void CObject::Release()
{
	//int nID = m_nID;
	//int nPrio = m_nPriority;

	m_bRelese = true;

	//if (m_apObject[nPrio][nID] != nullptr)
	//{
	//	delete m_apObject[nPrio][nID];
	//	m_apObject[nPrio][nID] = nullptr;
	//	m_nNumAll--;
	//}
}

void CObject::ReleaseAll()
{
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = m_pTop[nPrio];

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;
			if (pObj->m_type != FADE)
			{
				pObj->Release();
			}
			pObj = pNext;
		}
	}
	

	//// 全てを解放する処理
	//for (int nCntPrio = 0; nCntPrio < PRIORITY_MAX; nCntPrio++)
	//{
	//	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	//	{
	//		if (m_apObject[nCntPrio][nCnt] != nullptr)
	//		{
	//			if (m_apObject[nCntPrio][nCnt]->m_type != FADE)
	//			{
	//				m_apObject[nCntPrio][nCnt]->Uninit();
	//				delete m_apObject[nCntPrio][nCnt];
	//				m_apObject[nCntPrio][nCnt] = nullptr;
	//			}
	//			
	//		}
	//	}
	//}
}

void CObject::DeathAll()
{
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = m_pTop[nPrio];

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;
			if (pObj->m_bRelese)
			{

				// それぞれの自身の立ち位置によってつなげ方を変える
				if (pObj == m_pTop[nPrio] && pObj == m_pCur[nPrio])
				{ // ここに入った場合最後のオブジェクトとみなし、そのまま下の処理に持っていく
					m_pTop[nPrio] = nullptr;
					m_pCur[nPrio] = nullptr;
				}
				else if (pObj == m_pTop[nPrio])
				{
					m_pTop[nPrio] = pObj->m_pNext;	// 自身の次のポインタを top に設定する
					pObj->m_pNext->m_pPrev = nullptr; // 次のポインタの prev を nullptr にする
				}
				else if (pObj == m_pCur[nPrio])
				{
					m_pCur[nPrio] = pObj->m_pPrev;	// 自身の前のポインタを cur に設定する
					pObj->m_pPrev->m_pNext = nullptr;	// 前のポインタの next を nullptr にする
				}
				else
				{
					pObj->m_pPrev->m_pNext = pObj->m_pNext;	// 前のポインタの Next に自身の次のポインタの情報を入れる
					pObj->m_pNext->m_pPrev = pObj->m_pPrev;	// 次のポインタの Prev に自身の前のポインタの情報入れる
				}

				pObj->Uninit();
				delete pObj;
				pObj = nullptr;

			}
			pObj = pNext;

		}
	}
}

void CObject::Death()
{
	delete this;
}

void CObject::UpdateAll()
{
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = m_pTop[nPrio];

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;

			pObj->Update();

			pObj = pNext;
		}
	}

	DeathAll();	// 死亡フラグが立った object を delete する
	
	//for (int nCntPrio = 0; nCntPrio < PRIORITY_MAX; nCntPrio++)
	//{
	//	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	//	{
	//		if (m_apObject[nCntPrio][nCnt] != nullptr)
	//		{
	//			
	//			m_apObject[nCntPrio][nCnt]->Update();
	//		}
	//	}
	//}
}

void CObject::DrawAll()
{
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = m_pTop[nPrio];

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;

			pObj->Draw();

			pObj = pNext;
		}
	}

	
	//for (int nCntPrio = 0; nCntPrio < PRIORITY_MAX; nCntPrio++)
	//{
	//	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	//	{
	//		if (m_apObject[nCntPrio][nCnt] != nullptr)
	//		{
	//			m_apObject[nCntPrio][nCnt]->Draw();
	//		}
	//	}
	//}
}

CObject* CObject::GetOBJECT(int nPrio, int nIdx)
{
	//return m_apObject[nPrio][nIdx];

	return nullptr;
}

CObject::TYPE CObject::GetType()
{
	return m_type;
}

void CObject::SetType(TYPE type)
{
	m_type = type;
}
