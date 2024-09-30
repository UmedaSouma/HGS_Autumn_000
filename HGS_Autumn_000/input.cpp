#include "input.h"

// 静的メンバ変数の初期化
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
CInput::CInput()
{

}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
CInput::~CInput()
{
}

//===========================================================================================================
// 初期設定
//===========================================================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND /*hWnd*/)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInput, nullptr)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void CInput::Uninit()
{
	// 入力デバイス(キーボード)の破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// DirectInputオブジェクトの破棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void CInput::Update()
{
}

//===========================================================================================================
// キーボードのコンストラクタ
//===========================================================================================================
CInputKeyBoard::CInputKeyBoard() : m_aKeyState(), m_aKeyStateTrigger(), m_aKeyStateRelease()
{
}

//===========================================================================================================
// キーボードのデストラクタ
//===========================================================================================================
CInputKeyBoard::~CInputKeyBoard()
{
}

//===========================================================================================================
// キーボード初期設定
//===========================================================================================================
HRESULT CInputKeyBoard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//===========================================================================================================
// キーボードの終了処理
//===========================================================================================================
void CInputKeyBoard::Uninit()
{
	CInput::Uninit();
}

//===========================================================================================================
// キーボードのアップデート
//===========================================================================================================
void CInputKeyBoard::Update()
{
	BYTE aKeyState[MAX_KEY];	// キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			//g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			m_aKeyStateTrigger[nCntKey] = ~m_aKeyState[nCntKey] & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = ~aKeyState[nCntKey] & m_aKeyState[nCntKey];

			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//===========================================================
// キーボードのプレス情報を取得
//===========================================================
bool CInputKeyBoard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) != 0;
}

//===========================================================
// キーボードのトリガー情報を取得
//===========================================================
bool CInputKeyBoard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) != 0;
}

//===========================================================================================================
// キーボードのリリース処理
//===========================================================================================================
bool CInputKeyBoard::GetRelease(int Key)
{
	return (m_aKeyStateRelease[Key] & 0x80) != 0;
}

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
CInputJoypad::CInputJoypad()
{
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
CInputJoypad::~CInputJoypad()
{
}

HRESULT CInputJoypad::Init()
{
	for (int i = 0; i < PEDAL_MAX; i++)
	{
		m_TBtrigger[i] = false;
	}

	// メモリクリア
	memset(&m_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&m_joyKeyStateTrigger, 0, sizeof(XINPUT_STATE));
	memset(&m_joyKeyStateRelese, 0, sizeof(XINPUT_STATE));

	memset(&m_joyPadVibration, 0, sizeof(XINPUT_VIBRATION));

	// Xinputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

void CInputJoypad::Uninit()
{
	// XInputのステートを設定(無効にする)
	XInputEnable(false);
}

void CInputJoypad::Update()
{
	XINPUT_STATE joykeyState;	// ジョイパッドの入力情報

	// ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		m_joyTBpedal[PEDAL_LEFT] = joykeyState.Gamepad.bLeftTrigger;
		m_joyTBpedal[PEDAL_RIGHT] = joykeyState.Gamepad.bRightTrigger;
		joykeyState.Gamepad.wButtons |= ThumbToDPad(m_joyKeyState.Gamepad.sThumbLX, m_joyKeyState.Gamepad.sThumbLY, JOYPAD_DEADZONE);

		m_joyKeyStateRelese.Gamepad.wButtons = ~joykeyState.Gamepad.wButtons & m_joyKeyState.Gamepad.wButtons;	// リリース処理
		m_joyKeyStateTrigger.Gamepad.wButtons = (m_joyKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;	// トリガー処理
		m_joyTBpedalTrigger[PEDAL_LEFT] = (m_joyKeyState.Gamepad.bLeftTrigger ^ joykeyState.Gamepad.bLeftTrigger) & joykeyState.Gamepad.bLeftTrigger;
		m_joyTBpedalTrigger[PEDAL_RIGHT] = (m_joyKeyState.Gamepad.bRightTrigger ^ joykeyState.Gamepad.bRightTrigger) & joykeyState.Gamepad.bRightTrigger;

		m_joyKeyState = joykeyState;	// ジョイパッドのプレス情報を保存
	}
}

bool CInputJoypad::GetPress(JOYKEY key)
{
	return(m_joyKeyState.Gamepad.wButtons & (0x01 << key)) /*? true : false*/;
}

bool CInputJoypad::GetTrigger(JOYKEY key)
{
	return(m_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) /*? true : false*/;
}

bool CInputJoypad::GetRelese(JOYKEY key)
{
	return(m_joyKeyStateRelese.Gamepad.wButtons & (0x01 << key)) /*? true : false*/;
}

bool CInputJoypad::GetTriggerPedal(JOYKEY key)
{
	int KeyPedal = 0;	// トリガーボタンの数値を格納する
	int PedalNum = -1;	// トリガーボタンの種類を格納
	bool Trigger[PEDAL_MAX] = { false,false };	// トリガーボタンが押されたか確認する

	switch (key)
	{
	case JOYKEY_LEFT_TRIGGER:
		PedalNum = PEDAL_LEFT;
		break;

	case JOYKEY_RIGHT_TRIGGER:
		PedalNum = PEDAL_RIGHT;
		break;

	default:
		break;
	}

	if (m_joyTBpedal[PedalNum] >= PEDAL_DEADZONE)
	{// ペダルがこの数値以上下がったら

		KeyPedal = m_joyTBpedal[PedalNum];	// ペダルの数値を代入(のちに return で返す
		Trigger[PedalNum] = true;	// トリガーを引いた
	}

	if ((m_TBtrigger[PedalNum] ^ Trigger[PedalNum]) != 0)
	{// 前回トリガーを引いていなかったら
		m_TBtrigger[PedalNum] = Trigger[PedalNum];
		return KeyPedal > 0;
	}

	m_TBtrigger[PedalNum] = Trigger[PedalNum];
	return 0;
}

bool CInputJoypad::GetPressPedal(JOYKEY key)
{
	int KeyPedal = 0;	// トリガーボタンの数値を格納する
	int PedalNum = -1;	// トリガーボタンの種類を格納

	switch (key)
	{
	case JOYKEY_LEFT_TRIGGER:
		PedalNum = PEDAL_LEFT;
		break;

	case JOYKEY_RIGHT_TRIGGER:
		PedalNum = PEDAL_RIGHT;
		break;

	default:
		break;
	}

	KeyPedal = m_joyTBpedal[PedalNum];	// ペダルの数値を代入(のちに return で返す
	return KeyPedal > 0;
}

//===========================================================================================================
// スティックの処理
//===========================================================================================================
WORD CInputJoypad::ThumbToDPad(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone)
{
	WORD wButtons = 0;

	if (sThumbY >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_UP;
	}
	else if (sThumbY <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
	}

	if (sThumbX <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
	}
	else if (sThumbX >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
	}

	return wButtons;
}

////==========================================================================================
//// マウスの初期化処理
////==========================================================================================
//HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
//{
//	if (!CreateMouseDevice())
//	{
//		Uninit();
//		return E_FAIL;
//	}
//
//	ZeroMemory(&m_CurrentMouseState, sizeof(DIMOUSESTATE));
//	ZeroMemory(&m_PrevMouseState, sizeof(DIMOUSESTATE));
//
//	return S_OK;
//}
//
////==========================================================================================
//// マウスデバイスの作成
////==========================================================================================
//bool CInputMouse::CreateMouseDevice(void)
//{
//	// マウス用にデバイスオブジェクトを作成
//	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &g_MouseDevice, NULL)))
//	{
//		// デバイスの作成に失敗
//		return false;
//	}
//
//	// データフォーマットを設定
//	if (FAILED(g_MouseDevice->SetDataFormat(&c_dfDIMouse)))
//	{
//		// データフォーマットに失敗
//		return false;
//	}
//
//	// モードを設定（フォアグラウンド＆非排他モード）
//	if (FAILED(g_MouseDevice->SetCooperativeLevel(
//		FindWindowA(CLASS_NAME, NULL),
//		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
//	{
//		// モードの設定に失敗
//		return false;
//	}
//
//	// デバイスの設定
//	DIPROPDWORD diprop;
//	diprop.diph.dwSize = sizeof(diprop);
//	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
//	diprop.diph.dwObj = 0;
//	diprop.diph.dwHow = DIPH_DEVICE;
//	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）
//
//	if (FAILED(g_MouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
//	{
//		// デバイスの設定に失敗
//		return false;
//	}
//
//	// 入力制御開始
//	g_MouseDevice->Acquire();
//
//	return true;
//}

////===========================================================
//// ジョイパッドのプレス情報を取得
////===========================================================
//bool GetJoypadPress(JOYKEY key)
//{
//	return(g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
//}
//
////===========================================================
//// ジョイパッドのトリガー情報を取得
////===========================================================
//bool GetJoypadTrigger(JOYKEY key)
//{
//	return(g_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
//}

