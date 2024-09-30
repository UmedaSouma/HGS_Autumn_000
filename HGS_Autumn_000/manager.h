//===========================================================================================================================================================
// 
// manager.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "modeldata.h"
#include "scene.h"
#include "fade.h"
#include"d3dx9.h"

class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();

	static void SetMode(CScene::MODE mode);
private:
	static CScene* m_pScene;
	static CFade* m_pFade;

	static CRenderer* m_pRenderer;
	static CInputKeyBoard* m_pKeyboard;
	static CInputJoypad* m_pJoypad;
	static CTexture* m_pTexture;
	static CModeldata* m_pModeldata;
	static CLight* m_pLight;
	static CCamera* m_pCamera;

public:
	static CRenderer* GetRenderer() { return m_pRenderer; }			// レンダラーの取得
	static CInputKeyBoard* GetKeyboard() { return m_pKeyboard; }	// キーボードの取得
	static CInputJoypad* GetJoypad() { return m_pJoypad; }			// ジョイパッドの取得
	static CTexture* GetTexture() { return m_pTexture; }			// テクスチャの取得
	static CModeldata* GetModeldata() { return m_pModeldata; }		// モデルデータの取得
	static CFade* GetFade() { return m_pFade; }

	static CLight* GetLight() { return m_pLight; }					// ライトの取得
	static CCamera* GetCamera() { return m_pCamera; }				// カメラの取得

};

#endif // !_MANAGER_H_
