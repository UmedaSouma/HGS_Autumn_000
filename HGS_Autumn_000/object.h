//===========================================================================================================================================================
// 
// object.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

class CObject
{
public:

	typedef enum
	{
		NONE=0,
		ENEMY,			// 敵
		ENEMY_MESSE,	// 使役敵
		ENEMY_BOSS,
		PLAYER,			// プレイヤー
		BLOCK,			// ブロック
		MODEL,			// モデル
		FADE,			// フェード
		PARTICLE,		// パーティクル
		BASE,
		SWORD,			// 剣
		MAX
	}TYPE;

	// -- ほぼマクロ --
	static unsigned const int PRIORITY_DEFAULT = 3;	// 優先度のデフォルト
	static unsigned const int PRIORITY_MAX = 6;		// 優先度の最大値
	static unsigned const int MAX_OBJECT = 2048;	// オブジェクトの最大数

	CObject(int nPriority = PRIORITY_DEFAULT);
	virtual ~CObject();
	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	void Death();

	static void ReleaseAll();	// 全てを開放する
	static void UpdateAll();	// 全てを更新
	static void DrawAll();		// 全てを描画
	static void DeathAll();		// すべて削除

	TYPE GetType();				// タイプを取得
	void SetType(TYPE nType);	// タイプを設定
protected:
	void Release();	// 自分自身の解放
private:
	int m_nID;	// 自分自身のID
	int m_nPriority;	// 描画優先度
	TYPE m_type;

	static CObject* m_apObject[PRIORITY_MAX][MAX_OBJECT];	// オブジェクトリスト
	static int m_nNumAll;	// オブジェクトの総数

	// リストの変数
	static CObject*m_pObjectList;	// リスト構造のオブジェクトリスト
	static CObject* m_pTop[PRIORITY_MAX];			// リストの中の最初のポインタ
	static CObject* m_pCur[PRIORITY_MAX];			// リストの中の最後尾のポインタ
	CObject* m_pPrev;				// 自身の前のポインタ
	CObject* m_pNext;				// 次のポインタ
	bool m_bRelese;					// 解放フラグ
public:
	static CObject* GetOBJECT(int nPrio,int nIdx);	// オブジェクトを取得
	static CObject* GetTop(int Prio) { return m_pTop[Prio]; }

	CObject* GetNext() { return m_pNext; }
	CObject* GetPrev() { return m_pPrev; }
};

#endif // !_OBJECT_H_
