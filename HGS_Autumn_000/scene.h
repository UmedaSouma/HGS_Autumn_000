//===========================================================================================================================================================
// 
// scene.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "modeldata.h"

class CScene
{
public:
	typedef enum
	{
		MODE_NONE=0,
		MODE_TITLE,
		MODE_GAME,
		MODE_RESULT,
		MODE_EDITOR,
		MODE_MAX
	}MODE;

	CScene();
	virtual ~CScene();
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	static CScene* Create(MODE mode);
	static MODE GetMode();
private:
	static MODE m_Mode;	// 現在のモード


};


#endif // !_SCENE_H_