//===========================================================================================================================================================
// 
// ���f���̊Ǘ�
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "objectX.h"
#include "player3D.h"
#include "manager.h"

//===================================================================================
// �R���X�g���N�^
//===================================================================================
CObjectX::CObjectX(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ModelInfo.m_pTexture = nullptr;
}

//===================================================================================
// �f�X�g���N�^
//===================================================================================
CObjectX::~CObjectX()
{
	if (m_ModelInfo.m_pTexture != nullptr)
	{
		// �e�N�X�`���̔j��
		for (DWORD i = 0; i < m_ModelInfo.NumMat; i++)
		{
			if (m_ModelInfo.m_pTexture[i] != nullptr)
			{
				m_ModelInfo.m_pTexture[i]->Release();
			}
		}
		delete[]m_ModelInfo.m_pTexture;
		m_ModelInfo.m_pTexture = nullptr;
	}
}

//===================================================================================
// �����ݒ�
//===================================================================================
HRESULT CObjectX::Init()
{
	return S_OK;
}

//===================================================================================
// �I������
//===================================================================================
void CObjectX::Uninit()
{
	CPlayer3D* pPlayer3D = dynamic_cast<CPlayer3D*>(this);
	if (pPlayer3D == nullptr)
	{
		//if (m_ModelInfo.Mesh != nullptr)
		//{
		//	m_ModelInfo.Mesh->Release();
		//	m_ModelInfo.Mesh = nullptr;
		//}
	}

	//if (m_ModelInfo.BuffMat != nullptr)
	//{
	//	m_ModelInfo.BuffMat->Release();
	//	m_ModelInfo.BuffMat = nullptr;
	//}

	Release();
}

//===================================================================================
// �X�V����
//===================================================================================
void CObjectX::Update()
{

}

//===================================================================================
// �`�揈��
//===================================================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;				// �}�e���A���f�[�^�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (m_ModelInfo.BuffMat != nullptr
		&& m_ModelInfo.Mesh != nullptr
		&& m_ModelInfo.NumMat != 0)
	{
		// �}�e���A���f�[�^�̃|�C���^�̎擾
		pMat = (D3DXMATERIAL*)m_ModelInfo.BuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_ModelInfo.NumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_ModelInfo.m_pTexture[nCntMat]);

			// ���f���p�[�c�̕`��
			m_ModelInfo.Mesh->DrawSubset(nCntMat);
		}
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}


//===================================================================================
// �ʒu��ݒ�
//===================================================================================
void CObjectX::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===================================================================================
// �ʒu���擾
//===================================================================================
D3DXVECTOR3& CObjectX::GetPos()
{
	return m_pos;
}

//===================================================================================
// ���f���̏����Z�b�g����
//===================================================================================
void CObjectX::SetModelInfo(const char aPath[FILE_PATH])
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	pDevice = CManager::GetRenderer()->GetDevice();

	strcpy(&m_cFileName[0], &aPath[0]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(m_cFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_ModelInfo.BuffMat,
		NULL,
		&m_ModelInfo.NumMat,
		&m_ModelInfo.Mesh);

	// �}�e���A�����̎擾
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_ModelInfo.BuffMat->GetBufferPointer();

	m_pTexture = new LPDIRECT3DTEXTURE9[m_ModelInfo.NumMat];

	for (DWORD i = 0; i < m_ModelInfo.NumMat; i++)
	{
		m_pTexture[i] = nullptr;

		if (pMat[i].pTextureFilename)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[i].pTextureFilename, &m_pTexture[i]);
		}
	}

}

//===================================================================================
// ��������
//===================================================================================
CObjectX* CObjectX::Create(D3DXVECTOR3 pos)
{
	CObjectX* pModel = new CObjectX;

	pModel->SetPos(pos);
	pModel->SetType(TYPE::MODEL);

	pModel->Init();

	return pModel;
}