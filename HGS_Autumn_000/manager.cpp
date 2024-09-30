#include "manager.h"


// 静的メンバの初期化
CScene* CManager::m_pScene = nullptr;
CRenderer* CManager::m_pRenderer = nullptr;
CInputKeyBoard* CManager::m_pKeyboard = nullptr;
CInputJoypad* CManager::m_pJoypad = nullptr;
CTexture* CManager::m_pTexture = nullptr;
CModeldata* CManager::m_pModeldata = nullptr;
CFade* CManager::m_pFade = nullptr;
CLight* CManager::m_pLight = nullptr;
CCamera* CManager::m_pCamera = nullptr;

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
CManager::CManager()
{

}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
CManager::~CManager()
{

}

//===========================================================================================================
// 初期設定
//===========================================================================================================
HRESULT CManager::Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow)
{

	srand((unsigned int)time(NULL)); // 現在時刻の情報で初期化

	m_pKeyboard = new CInputKeyBoard;
	m_pJoypad = new CInputJoypad;
	m_pRenderer = new CRenderer;
	m_pTexture = new CTexture;
	m_pModeldata = new CModeldata;
	m_pFade = new CFade;

	m_pCamera = new CCamera;
	m_pLight = new CLight;

	m_pRenderer->Init(hWnd, bWindow);
	m_pKeyboard->Init(hinstance, hWnd);
	m_pJoypad->Init();
	m_pFade->Init();

	m_pCamera->Init();
	m_pLight->Init();

	m_pFade->SetFade(CScene::MODE_TITLE);
	//SetMode(CScene::MODE_TITLE);

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void CManager::Uninit()
{
	//TextureUnLoadAll();

	m_pTexture->Unload();
	m_pModeldata->Unload();

	m_pFade->Uninit();

	m_pJoypad->Uninit();
	m_pKeyboard->Uninit();

	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
	}

	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pLight != nullptr)
	{
		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pCamera != nullptr)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	CObject::DeathAll();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void CManager::Update()
{

	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	//if (m_pFade->GetState() != CFade::FADE_OUT)
	{
		if (m_pScene != nullptr)
		{
			m_pScene->Update();
		}
	}

	m_pCamera->Update();
	m_pKeyboard->Update();
	m_pJoypad->Update();

	//m_pFade->Update();

}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void CManager::Draw()
{
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}

	if (m_pFade->GetState() == CFade::FADE_NONE)
	{
		if (m_pScene != nullptr)
		{
			m_pScene->Draw();
		}
	}

	//m_pFade->Draw();
}

//===========================================================================================================
// モード設定
//===========================================================================================================
void CManager::SetMode(CScene::MODE mode)
{
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	m_pScene = CScene::Create(mode);
	m_pScene->Init();
}

