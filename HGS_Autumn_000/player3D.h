//===========================================================================================================================================================
// 
// player3D.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _PLAYER_3D_H_
#define _PLAYER_3D_H_
#include "main.h"
#include "model.h"

//===================================================================================
// クラス
//===================================================================================

// Player3Dのクラス
class CPlayer3D :public CModel
{
private:
	typedef enum
	{
		COLLISION_HORIZON = 0,
		COLLISION_VERTICAL, 
		COLLISION_HIGHLOW,
		COLLISION_MAX
	}COLLISION_TYPE;

	static inline const float m_fJumpPower = 8.0f;	// ジャンプ力
	static inline const float m_fGravity = 0.5f;	// 重力
	static const unsigned int m_MaxLife = 560;		// 体力
	static const unsigned int m_MaxInterval = 15;	// ダメージを受ける間隔
public:

	// プレイヤーのスキル状態
	typedef enum
	{
		SKILL_NONE = 0,
		SKILL_TRIGGER,		// スキルボタンが押された瞬間
		SKILL_PRESS,		// 押されている間
		SKILL_RELEASE,		// 離されたとき
		SKILL_CANCELL,		// スキル解除
		SKILL_SWORD,		// 剣スキル発動
		SKILL_SWORD_CANCEL,	// 剣スキル解除
		SKILL_MAX
	}SKILL_STATE;

	CPlayer3D(int nPriority = PRIORITY_DEFAULT-1);			// コンストラクタ
	~CPlayer3D()override;			// デストラクタ
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了処理
	void Update()override;	// 更新処理
	void Draw()override;	// 描画処理

	D3DXVECTOR3 InputPosPlayer();
	void SetTargetCamera();	// player の pos を camera の target にセットする
	void Jump();
	void UpdateDamage();		// ダメージを受けているときの処理
	void DeathPlayer();			// プレイヤーが死んだとき

	D3DXVECTOR3 LookObjectCollision(COLLISION_TYPE type);
	D3DXVECTOR3 VerticalCollision(CObject* pObj);	// 縦の当たり判定
	D3DXVECTOR3 HorizonCollision(CObject* pObj);	// 横の当たり判定
	D3DXVECTOR3 HighLowCollision(CObject* pObj);	// 上下の当たり判定

	CPlayer3D* GetPlayer() { return this; }
	static CPlayer3D* Create(D3DXVECTOR3 pos);	// 生成処理
private:
	float m_fSpeed;				// プレイヤーの速度
	bool m_bUseJump;			// ジャンプを使っているかどうかの判定(無限ジャンプしないため)
	D3DXVECTOR3 m_Move;			// 移動処理
	D3DXVECTOR3 m_oldPos;		// 1フレーム前のpos
	SKILL_STATE m_skillstate;	// スキルボタンを押すときの状態
	static int m_nFragmentNum;	// 現在持っている欠片の数
	
	bool m_bSlip;
	int m_nSlipCnt;
	bool m_bDammage;	// ダメージ状態を示す bool
	int m_nMaxLife = m_MaxLife;
	int m_nLife;		// 体力
	int m_nCntDamage;	// ダメージ状態のカウント
	
public:
	SKILL_STATE GetSkillstate(){ return m_skillstate; }	// スキルボタンの状態

	void SetMove(D3DXVECTOR3 move) { m_Move = move; }	// move の数値を設定する
	D3DXVECTOR3 GetMove() { return m_Move; }			// move の数値を返す

	D3DXVECTOR3 GetOldPos() { return m_oldPos; }		// 1フレーム前の pos を取得する
	void SetbUseJump(bool use) { m_bUseJump = use; }	// ジャンプの状態を設定する

	void AddDamage(int nDamage);	// ダメージ増やす処理

	void SetLife(int life) { m_nLife = life; }
	int GetLife() { return m_nLife; }

	int GetMaxLife() { return m_MaxLife; }
};


#endif // !_PLAYER_3D_H_