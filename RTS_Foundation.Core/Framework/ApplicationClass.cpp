#include "ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_Input = nullptr;
	m_Direct3D = nullptr;
	m_Timer = nullptr;
	m_Fps = nullptr;
	m_Cpu = nullptr;
	m_ShaderManager = nullptr;
	m_Zone = nullptr;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;

	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	result = m_Direct3D->Initialize(screenWidth, screenHeight, SettingsHelperClass::VSYNC_ENABLED, hwnd, SettingsHelperClass::FULL_SCREEN, SettingsHelperClass::SCREEN_DEPTH, SettingsHelperClass::SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	m_ShaderManager = new ShaderManagerClass;
	if (!m_ShaderManager)
	{
		return false;
	}

	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	m_TextureManager = new TextureManagerClass;
	if (!m_TextureManager)
	{
		return false;
	}

	result = m_TextureManager->Initialize(15);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture manager object.", L"Error", MB_OK);
		return false;
	}

	//result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_DEBUG.jpg", 0);
	//if (!result)
	//{
	//	return false;
	//}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_SAND_001.jpg", 1);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_GRASS_002.jpg", 2);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_GRASS_001.jpg", 3);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_STONE_001.jpg", 4);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_STONE_003.jpg", 5);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_STONE_002.jpg", 6);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_SNOW_001.jpg", 7);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_CHRYSANTEMUM_PATTERN.jpg", 11);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_HEARTS.jpg", 12);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_GLITTER_GOLD.jpg", 13);
	if (!result)
	{
		return false;
	}

	result = m_TextureManager->LoadTexture(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"Resources/Textures/TEX_GLITTER_SILVER.jpg", 14);
	if (!result)
	{
		return false;
	}

	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	m_Fps = new FPSClass;
	if (!m_Fps)
	{
		return false;
	}

	m_Fps->Initialize();

	m_Cpu = new CPUClass;
	if (!m_Cpu)
	{
		return false;
	}

	m_Cpu->Initialize();

	m_Zone = new ZoneClass;
	if (!m_Zone)
	{
		return false;
	}

	result = m_Zone->Initialize(m_Direct3D, hwnd, screenWidth, screenHeight, m_TextureManager);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the zone object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown()
{
	if (m_Zone)
	{
		m_Zone->Shutdown();
		delete m_Zone;
		m_Zone = nullptr;
	}

	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = nullptr;
	}

	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = nullptr;
	}

	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = nullptr;
	}

	if (m_TextureManager)
	{
		m_TextureManager->Shutdown();
		delete m_TextureManager;
		m_TextureManager = nullptr;
	}

	if (m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = nullptr;
	}

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}

	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = nullptr;
	}

	return;
}

bool ApplicationClass::Frame()
{
	bool result;

	m_Fps->Frame();
	m_Cpu->Frame();
	m_Timer->Frame();

	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	if (m_Input->KeyPressed(DIK_ESCAPE) == true)
	{
		return false;
	}
	
	if (m_Input->KeyDown(DIK_Z) == true)
	{
		m_Direct3D->ToggleFillMode();
	}

	result = m_Zone->Frame(m_Direct3D, m_Input, m_ShaderManager, m_TextureManager, m_Timer->GetTime(), m_Fps->GetFps(), m_Cpu->GetCpuPercentage());
	if (!result)
	{
		return false;
	}

	return result;
}
