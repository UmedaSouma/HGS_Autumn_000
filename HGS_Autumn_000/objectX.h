//===========================================================================================================================================================
// 
// model.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _OBJEXTX_H_
#define _OBJEXTX_H_

#include "main.h"
#include "object.h"

class CObjectX :public CObject
{
private:
	typedef struct
	{
		LPD3DXMESH Mesh;			// ���b�V��(���_���)�ւ̃|�C���^
		LPD3DXBUFFER BuffMat;	// �}�e���A���ւ̃|�C���^
		DWORD NumMat;	// �}�e���A���̐�
		LPDIRECT3DTEXTURE9* m_pTexture;	// �e�N�X�`���̃|�C���^
	}ModelInfo;

	ModelInfo m_ModelInfo;
public:
	CObjectX(int nPriority = 3);
	~CObjectX();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3& GetPos();
	
	const static int FILE_PATH = 128;											// �t�@�C���p�X�̍ő啶����

	void SetModelInfo(const char aPath[FILE_PATH]);

	static CObjectX* Create(D3DXVECTOR3 pos);


private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_scale;
	D3DXMATRIX m_mtxWorld;

	char m_cFileName[FILE_PATH];												// X�t�@�C���̃p�X
	LPDIRECT3DTEXTURE9* m_pTexture;												// �e�N�X�`���̏��

};

#endif // !_MODEL_H_
