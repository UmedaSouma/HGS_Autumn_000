//===========================================================================================================================================================
// 
// fade.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _FADE_H_
#define _FADE_H_
#include "main.h"
#include "object2D.h"
#include "scene.h"

class CFade :public CObject2D
{
public:
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_STOP,
		FADE_MAX
	}STATE;

	//CFade();
	CFade(int nPriority = PRIORITY_MAX-1);
	~CFade()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画
	void SetFade(CScene::MODE nextmode);
	void FadeIn();
	void FadeOut();
	STATE GetState();
private:
	STATE m_State;
	CScene::MODE m_NextMode;
	int m_fFadeframe;	// フェード遷移の時間
	//CFade* m_pState;
};



#endif // !_FADE_H_