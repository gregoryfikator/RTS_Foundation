#include "UIClass.h"

UIClass::UIClass()
{
	m_Font = nullptr;

	m_VideoStrings = nullptr;
	m_FpsString = nullptr;
	m_CpuString = nullptr;
	m_MousePositionString = nullptr;
	m_PositionStrings = nullptr;
	m_RotationStrings = nullptr;

	m_GeneratorStrings = nullptr;
}

UIClass::UIClass(const UIClass& other)
{
}

UIClass::~UIClass()
{
}

bool UIClass::Initialize(D3DClass* Direct3D, HWND hwnd, int screenHeight, int screenWidth)
{
	bool result;
	char videoCard[128];
	int videoMemory;
	char videoString[144];
	char memoryString[32];
	char tempString[16];
	int i;

	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}

	result = m_Font->Initialize(Direct3D->GetDevice(), "Resources/Fonts/fontdata.txt", L"Resources/Fonts/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Setup the video card strings.
	Direct3D->GetVideoCardInfo(videoCard, videoMemory);
	strcpy_s(videoString, "Video Card: ");
	strcat_s(videoString, videoCard);

	_itoa_s(videoMemory, tempString, 10);

	strcpy_s(memoryString, "Video Memory: ");
	strcat_s(memoryString, tempString);
	strcat_s(memoryString, " MB");

	// Create the text objects for the video strings.
	m_VideoStrings = new TextClass[2];
	if (!m_VideoStrings)
	{
		return false;
	}

	// Initialize the video text strings.
	result = m_VideoStrings[0].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 256, false, m_Font,
		videoString, 8, 8, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_VideoStrings[1].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, m_Font,
		memoryString, 8, 28, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Create the text object for the fps string.
	m_FpsString = new TextClass;
	if (!m_FpsString)
	{
		return false;
	}

	// Initialize the fps text string.
	result = m_FpsString->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font,
		"FPS: 0", 8, 52, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	m_previousFps = -1;

	m_CpuString = new TextClass;
	if (!m_CpuString)
	{
		return false;
	}

	// Initialize the fps text string.
	result = m_CpuString->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font,
		"CPU: 0%", 8, 72, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	m_previousCpu = -1;

	m_MousePositionString = new TextClass;
	if (!m_MousePositionString)
	{
		return false;
	}

	// Initialize the fps text string.
	result = m_MousePositionString->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 64, false, m_Font,
		"Cursor Position: 0, 0", 8, 100, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	for (i = 0; i < 2; i++)
	{
		m_previousMousePosition[i] = 0;
	}

	m_renderCountString = new TextClass;
	if (!m_renderCountString)
	{
		return false;
	}

	result = m_renderCountString->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 64, false, m_Font,
		"Frustum Cull Count: 0/0", 8, 120, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	m_previousRenderCount = 0;
	m_previousRenderMaxCount = 0;

	// Create the text objects for the position strings.
	m_PositionStrings = new TextClass[4];
	if (!m_PositionStrings)
	{
		return false;
	}

	result = m_PositionStrings[0].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, m_Font,
		"Camera Translation", 8, 140, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	result = m_PositionStrings[1].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font,
		"X: 0", 8, 160, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_PositionStrings[2].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font,
		"Y: 0", 8, 180, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_PositionStrings[3].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font,
		"Z: 0", 8, 200, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	for (i = 0; i < 3; i++)
	{
		m_previousPosition[i] = -1;
	}

	m_RotationStrings = new TextClass[4];
	if (!m_RotationStrings)
	{
		return false;
	}

	result = m_RotationStrings[0].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, false, m_Font,
		"Camera Rotation", 8, 220, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	result = m_RotationStrings[1].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font,
		"X: 0", 8, 240, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_RotationStrings[2].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font,
		"Y: 0", 8, 260, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_RotationStrings[3].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 16, false, m_Font,
		"Z: 0", 8, 280, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	for (i = 0; i < 3; i++)
	{
		m_previousRotation[i] = -1;
	}

	m_GeneratorStrings = new TextClass[6];
	if (!m_GeneratorStrings)
	{
		return false;
	}


	result = m_GeneratorStrings[0].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 64, false, m_Font,
		"Generator [+ and - to change, SPACE to reset]", 8, 300, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	result = m_GeneratorStrings[1].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 64, false, m_Font,
		"Generated in 0ms", 8, 320, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_GeneratorStrings[2].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 64, false, m_Font,
		"Seed [LCTRL + 1]: -1", 8, 340, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	m_previousGeneratorSeed = -1;

	result = m_GeneratorStrings[3].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 64, false, m_Font,
		"Displacement [LCTRL + 2 + NUM_2468]: 0.0 0.0", 8, 360, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	for (i = 0; i < 2; i++)
	{
		m_previousGeneratorDisplacement[i] = 0.0f;
	}

	result = m_GeneratorStrings[4].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 64, false, m_Font,
		"Frequency [LCTRL + 3]: 2.0", 8, 380, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	m_previousGeneratorFrequency = 2.0f;

	result = m_GeneratorStrings[5].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 64, false, m_Font,
		"Terrace smoothness [LCTRL + 4]: 5.0", 8, 400, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	m_previousGeneratorTerraceSmoothness = 5.0f;

	result = m_GeneratorStrings[6].Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), screenWidth, screenHeight, 64, false, m_Font,
		"Exponent [LCTRL + 5]: 2.75", 8, 420, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	m_previousGeneratorExponent = 2.75f;

	m_CursorBitmap = new BitmapClass;
	if (!m_CursorBitmap)
	{
		return false;
	}

	result = m_CursorBitmap->Initialize(Direct3D->GetDevice(), screenWidth, screenHeight, L"Resources/Textures/mouse.dds", 32, 32);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void UIClass::Shutdown()
{
	if (m_GeneratorStrings)
	{
		m_GeneratorStrings[0].Shutdown();
		m_GeneratorStrings[1].Shutdown();
		m_GeneratorStrings[2].Shutdown();
		m_GeneratorStrings[3].Shutdown();
	}

	if (m_RotationStrings)
	{
		m_RotationStrings[0].Shutdown();
		m_RotationStrings[1].Shutdown();
		m_RotationStrings[2].Shutdown();
		m_RotationStrings[3].Shutdown();

		delete[] m_RotationStrings;
		m_RotationStrings = nullptr;
	}

	if (m_PositionStrings)
	{
		m_PositionStrings[0].Shutdown();
		m_PositionStrings[1].Shutdown();
		m_PositionStrings[2].Shutdown();
		m_PositionStrings[3].Shutdown();

		delete[] m_PositionStrings;
		m_PositionStrings = nullptr;
	}

	if (m_renderCountString)
	{
		m_renderCountString->Shutdown();
		delete m_renderCountString;
		m_renderCountString = nullptr;
	}

	if (m_MousePositionString)
	{
		m_MousePositionString->Shutdown();
		delete m_MousePositionString;
		m_MousePositionString = nullptr;
	}

	if (m_CpuString)
	{
		m_CpuString->Shutdown();
		delete m_CpuString;
		m_CpuString = nullptr;
	}

	if (m_FpsString)
	{
		m_FpsString->Shutdown();
		delete m_FpsString;
		m_FpsString = nullptr;
	}

	if (m_VideoStrings)
	{
		m_VideoStrings[0].Shutdown();
		m_VideoStrings[1].Shutdown();

		delete[] m_VideoStrings;
		m_VideoStrings = nullptr;
	}

	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = nullptr;
	}

	return;
}

bool UIClass::Frame(ID3D11DeviceContext* deviceContext, int fps, int cpuUsage, int mousePosX, int mousePosY, int renderCount, int renderMaxCount, float posX, float posY, float posZ,
	float rotX, float rotY, float rotZ, TerrainClass* Terrain)
{
	bool result;

	result = SetFps(fps, deviceContext);
	if (!result)
	{
		return false;
	}

	result = SetCpu(cpuUsage, deviceContext);
	if (!result)
	{
		return false;
	}

	result = SetMousePosition(mousePosX, mousePosY, deviceContext);
	if (!result)
	{
		return false;
	}

	result = SetRenderCount(renderCount, renderMaxCount, deviceContext);
	if (!result)
	{
		return false;
	}

	result = SetCameraPosition(posX, posY, posZ, deviceContext);
	if (!result)
	{
		return false;
	}

	result = SetCameraRotation(rotX, rotY, rotZ, deviceContext);
	if (!result)
	{
		return false;
	}

	result = SetGeneratorParameters(Terrain, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX orthoMatrix, bool display_ui)
{
	int i;
	bool result;

	// Turn off the Z buffer and enable alpha blending to begin 2D rendering.
	Direct3D->TurnZBufferOff();
	Direct3D->TurnAlphaBlendingOn();

	if (display_ui == true)
	{
		m_VideoStrings[0].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture());
		m_VideoStrings[1].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture());

		m_FpsString->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture());

		m_CpuString->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture());

		m_MousePositionString->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture());

		m_renderCountString->Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture());

		for (i = 0; i < 4; i++)
		{
			m_PositionStrings[i].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture());
		}

		for (i = 0; i < 4; i++)
		{
			m_RotationStrings[i].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture());
		}

		for (i = 0; i < 7; i++)
		{
			m_GeneratorStrings[i].Render(Direct3D->GetDeviceContext(), ShaderManager, worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture());
		}
	}

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_CursorBitmap->Render(Direct3D->GetDeviceContext(), m_previousMousePosition[0], m_previousMousePosition[1]);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = ShaderManager->RenderTextureShader(Direct3D->GetDeviceContext(), m_CursorBitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_CursorBitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending now that the text has been rendered.
	Direct3D->TurnAlphaBlendingOff();

	// Turn the Z buffer back on now that the 2D rendering has completed.
	Direct3D->TurnZBufferOn();

	return true;
}

bool UIClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char finalString[16];
	float red, green, blue;
	bool result;

	// Check if the fps from the previous frame was the same, if so don't need to update the text string.
	if (m_previousFps == fps)
	{
		return true;
	}

	// Store the fps for checking next frame.
	m_previousFps = fps;

	// Truncate the fps to below 100,000.
	if (fps > 99999)
	{
		fps = 99999;
	}

	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup the fps string.
	strcpy_s(finalString, "FPS: ");
	strcat_s(finalString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = m_FpsString->UpdateSentence(deviceContext, m_Font, finalString, 8, 52, red, green, blue);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;

	if (m_previousCpu == cpu)
	{
		return true;
	}

	m_previousCpu = cpu;

	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);

	// Setup the cpu string.
	strcpy_s(cpuString, "CPU: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update the sentence vertex buffer with the new string information.
	result = m_CpuString->UpdateSentence(deviceContext, m_Font, cpuString, 8, 72, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[64];
	char mouseString[64];
	bool result;

	if (m_previousMousePosition[0] == mouseX && m_previousMousePosition[1] == mouseY)
	{
		return true;
	}

	m_previousMousePosition[0] = mouseX;
	m_previousMousePosition[1] = mouseY;

	_itoa_s(mouseX, tempString, 10);

	strcpy_s(mouseString, "Cursor Position: ");
	strcat_s(mouseString, tempString);
	strcat_s(mouseString, ", ");

	_itoa_s(mouseY, tempString, 10);
	strcat_s(mouseString, tempString);

	result = m_MousePositionString->UpdateSentence(deviceContext, m_Font, mouseString, 8, 100, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::SetRenderCount(int count, int maxCount, ID3D11DeviceContext* deviceContext)
{
	char tempString1[16];
	char tempString2[16];
	char countString[64];
	bool result;

	if (m_previousRenderCount == count && m_previousRenderMaxCount == maxCount)
	{
		return true;
	}

	m_previousRenderCount = count;
	m_previousRenderMaxCount = maxCount;

	// Convert the count integer to string format.
	_itoa_s(count, tempString1, 10);
	_itoa_s(maxCount, tempString2, 10);

	// Setup the render count string.
	strcpy_s(countString, "Frustum Cull Count: ");
	strcat_s(countString, tempString1);
	strcat_s(countString, "/");
	strcat_s(countString, tempString2);

	// Update the sentence vertex buffer with the new string information.
	result = m_renderCountString->UpdateSentence(deviceContext, m_Font, countString, 8, 120, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

bool UIClass::SetCameraPosition(float posX, float posY, float posZ, ID3D11DeviceContext* deviceContext)
{
	int positionX, positionY, positionZ;
	char tempString[16];
	char finalString[16];
	bool result;

	// Convert the float values to integers.
	positionX = (int)posX;
	positionY = (int)posY;
	positionZ = (int)posZ;

	// Update the position strings if the value has changed since the last frame.
	if (positionX != m_previousPosition[0])
	{
		m_previousPosition[0] = positionX;
		_itoa_s(positionX, tempString, 10);
		strcpy_s(finalString, "X: ");
		strcat_s(finalString, tempString);
		result = m_PositionStrings[1].UpdateSentence(deviceContext, m_Font, finalString, 8, 160, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	if (positionY != m_previousPosition[1])
	{
		m_previousPosition[1] = positionY;
		_itoa_s(positionY, tempString, 10);
		strcpy_s(finalString, "Y: ");
		strcat_s(finalString, tempString);
		result = m_PositionStrings[2].UpdateSentence(deviceContext, m_Font, finalString, 8, 180, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	if (positionZ != m_previousPosition[2])
	{
		m_previousPosition[2] = positionZ;
		_itoa_s(positionZ, tempString, 10);
		strcpy_s(finalString, "Z: ");
		strcat_s(finalString, tempString);
		result = m_PositionStrings[3].UpdateSentence(deviceContext, m_Font, finalString, 8, 200, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	return true;
}

bool UIClass::SetCameraRotation(float rotX, float rotY, float rotZ, ID3D11DeviceContext* deviceContext)
{
	int rotationX, rotationY, rotationZ;
	char tempString[16];
	char finalString[16];
	bool result;

	// Convert the float values to integers.

	rotationX = (int)rotX;
	rotationY = (int)rotY;
	rotationZ = (int)rotZ;

	if (rotationX != m_previousRotation[0])
	{
		m_previousRotation[0] = rotationX;
		_itoa_s(rotationX, tempString, 10);
		strcpy_s(finalString, "X: ");
		strcat_s(finalString, tempString);
		result = m_RotationStrings[1].UpdateSentence(deviceContext, m_Font, finalString, 8, 240, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	if (rotationY != m_previousRotation[1])
	{
		m_previousRotation[1] = rotationY;
		_itoa_s(rotationY, tempString, 10);
		strcpy_s(finalString, "Y: ");
		strcat_s(finalString, tempString);
		result = m_RotationStrings[2].UpdateSentence(deviceContext, m_Font, finalString, 8, 260, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	if (rotationZ != m_previousRotation[2])
	{
		m_previousRotation[2] = rotationZ;
		_itoa_s(rotationZ, tempString, 10);
		strcpy_s(finalString, "Z: ");
		strcat_s(finalString, tempString);
		result = m_RotationStrings[3].UpdateSentence(deviceContext, m_Font, finalString, 8, 280, 1.0f, 1.0f, 1.0f);
		if (!result) { return false; }
	}

	return true;
}

bool UIClass::SetGeneratorParameters(TerrainClass* Terrain, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char resultString[64];
	bool result;

	long generationTime = Terrain->GetGenerationTime();
	long seed = Terrain->GetSeed();
	float* displacement = Terrain->GetDisplacement();
	float frequency = Terrain->GetFrequency();
	float terraceSmoothness = Terrain->GetTerraceSmoothness();
	float exponent = Terrain->GetExponent();

	if (m_previousGenerationTime != generationTime)
	{
		m_previousGenerationTime = generationTime;

		_ltoa_s(m_previousGenerationTime, tempString, 10);

		strcpy_s(resultString, "Generated in ");
		strcat_s(resultString, tempString);
		strcat_s(resultString, "ms");

		result = m_GeneratorStrings[1].UpdateSentence(deviceContext, m_Font, resultString, 8, 320, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	if (m_previousGeneratorSeed != seed)
	{
		m_previousGeneratorSeed = seed;

		_ltoa_s(m_previousGeneratorSeed, tempString, 10);

		strcpy_s(resultString, "Seed [LCTRL + 1]: ");
		strcat_s(resultString, tempString);

		result = m_GeneratorStrings[2].UpdateSentence(deviceContext, m_Font, resultString, 8, 340, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	if (m_previousGeneratorDisplacement[0] != displacement[0] || m_previousGeneratorDisplacement[1] != displacement[1])
	{
		m_previousGeneratorDisplacement[0] = displacement[0];
		m_previousGeneratorDisplacement[1] = displacement[1];

		std::stringstream stream;

		stream << std::fixed << std::setprecision(2) << displacement[0] << " " << displacement[1];

		strcpy_s(resultString, "Displacement [LCTRL + 2 + NUM_2468]: ");
		strcat_s(resultString, stream.str().c_str());

		result = m_GeneratorStrings[3].UpdateSentence(deviceContext, m_Font, resultString, 8, 360, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	if (m_previousGeneratorFrequency != frequency)
	{
		m_previousGeneratorFrequency = frequency;

		std::stringstream stream;

		stream << std::fixed << std::setprecision(2) << frequency;

		strcpy_s(resultString, "Frequency [LCTRL + 3]: ");
		strcat_s(resultString, stream.str().c_str());

		result = m_GeneratorStrings[4].UpdateSentence(deviceContext, m_Font, resultString, 8, 380, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	if (m_previousGeneratorTerraceSmoothness != terraceSmoothness)
	{
		m_previousGeneratorTerraceSmoothness = terraceSmoothness;

		std::stringstream stream;

		stream << std::fixed << std::setprecision(2) << terraceSmoothness;

		strcpy_s(resultString, "Terrace smoothness [LCTRL + 4]: ");
		strcat_s(resultString, stream.str().c_str());

		result = m_GeneratorStrings[5].UpdateSentence(deviceContext, m_Font, resultString, 8, 400, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}

	if (m_previousGeneratorExponent != exponent)
	{
		m_previousGeneratorExponent = exponent;

		std::stringstream stream;

		stream << std::fixed << std::setprecision(2) << exponent;

		strcpy_s(resultString, "Exponent [LCTRL + 5]: ");
		strcat_s(resultString, stream.str().c_str());

		result = m_GeneratorStrings[6].UpdateSentence(deviceContext, m_Font, resultString, 8, 420, 1.0f, 1.0f, 1.0f);
		if (!result)
		{
			return false;
		}
	}


	return true;
}