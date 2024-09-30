//===========================================================================================================================================================
// 
// game.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "main.h"
#include "scene.h"

class CGame :public CScene
{
public:
	CGame();
	~CGame()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetInitUI();	// uiのセット
	static void UpdateDelay();
	static void SetDelay() { m_Delay = true; m_nDelayEnd = 60;}
private:
	static int m_nDelayEnd;	// 倒してからリザルトになるまで
	static bool m_Delay;

public:
};