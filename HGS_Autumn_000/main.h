//===========================================================================================================================================================
// 
// main.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _MAIN_H_
#define _MAIN_H_


#include <vector>
#include <unordered_map>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"xinput.lib")	// ジョイパッド処理に必要

//#pragma warning (disable: 4100)

#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE| D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// マクロ定義
#define CLASS_NAME		 "WindowClass"			// ウィンドウクラスの名前
#define WINDOW_NAME		 "DirectX基本処理"	// ウィンドウの名前
#define ID_BUTTON_FINISH (101)

//using namespace std;

// 頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;

// 頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_3D;

class CMain
{
public:
	CMain();
	~CMain();
	static void ToggleFullscreen(HWND hWnd);
private:
	static bool m_IsFullScreem;
	static RECT m_WindowRect;
};

// メモリリークの検出
#include <crtdbg.h>
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#endif // !_MAIN_H_

