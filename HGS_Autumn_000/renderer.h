//===========================================================================================================================================================
// 
// renderer.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"d3dx9.h"

class CRenderer
{
private:

	typedef enum
	{
		_FADE_ALPHA=0,
		_ENEMY_POS,
		_ENEMY_MOVE,
		_PLAYER_POS,
		_PLAYER_MOVE,
		_ENEMY_BUTTLE,
		_MAX_PRINT
	}_DEBUG_PRINT;

public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd,BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice();	// 3D�f�o�C�X�̎擾

	void DebugPrint(char&pStr, char& pStr2);
private:
	LPDIRECT3D9 m_pD3D;	// Direct3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// Direct3D�f�o�C�X
	LPD3DXFONT m_pFont;	// �t�H���g�̃|�C���^
};

#endif // !_RENDERER_H_