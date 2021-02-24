#include "ZoneClass.h"

ZoneClass::ZoneClass()
{
	m_Camera = nullptr;
	m_Frustum = nullptr;
	m_Light = nullptr;
	m_Model = nullptr;
	m_ModelList = nullptr;
	m_Position = nullptr;
	m_Terrain = nullptr;
	m_UI = nullptr;

	m_objLoader = nullptr;
}

ZoneClass::ZoneClass(const ZoneClass & other)
{
}

ZoneClass::~ZoneClass()
{
}

bool ZoneClass::Initialize(D3DClass * Direct3D, HWND hwnd, int screenWidth, int screenHeight, TextureManagerClass* TextureManager)
{
	bool result;

	// Create the user interface object.
	m_UI = new UIClass;
	if (!m_UI)
	{
		return false;
	}

	// Initialize the user interface object.
	result = m_UI->Initialize(Direct3D, hwnd, screenHeight, screenWidth);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera and build the matrices needed for rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->RenderBaseViewMatrix();

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-0.5f, -1.0f, -0.5f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(4.0f);

	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	m_Position->SetPosition(128.0f, 15.0f, 118.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);
	
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	result = m_Terrain->Initialize(Direct3D->GetDevice(), "Resources/Levels/level_001.txt", TextureManager);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	m_ModelList = new ModelListClass;
	if (!m_ModelList)
	{
		return false;
	}

	result = m_ModelList->Initialize(25);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	m_objLoader = new OBJLoader;
	if (!m_objLoader)
	{
		return false;
	}

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(Direct3D->GetDevice(), m_objLoader, "Resources/Models/Pawn4.obj", TextureManager);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_displayUI = true;

	return true;
}

void ZoneClass::Shutdown()
{
	if (m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = nullptr;
	}

	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = nullptr;
	}

	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = nullptr;
	}

	if (m_Position)
	{
		delete m_Position;
		m_Position = nullptr;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = nullptr;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	if (m_UI)
	{
		m_UI->Shutdown();
		delete m_UI;
		m_UI = nullptr;
	}

	return;
}

bool ZoneClass::Frame(D3DClass * Direct3D, InputClass * input, ShaderManagerClass * ShaderManager, TextureManagerClass* TextureManager, float frameTime, int fps, int cpuUsage)
{
	bool result;
	int mousePosX, mousePosY;
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Do the frame input processing.
	HandleInput(Direct3D->GetDevice(), input, frameTime);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	input->GetMouseLocation(mousePosX, mousePosY);

	int renderCount = 0, renderMaxCount = 25;
	// Do the frame processing for the user interface.
	result = m_UI->Frame(Direct3D->GetDeviceContext(), fps, cpuUsage, mousePosX, mousePosY, renderCount, renderMaxCount, posX, posY, posZ, rotX, rotY, rotZ, m_Terrain);
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render(Direct3D, ShaderManager, TextureManager);
	if (!result)
	{
		return false;
	}

	return true;
}

void ZoneClass::HandleInput(ID3D11Device* device, InputClass* input, float frameTime)
{
	HandleMovementInput(input, frameTime);

	HandleGeneratorInput(device, input);

	// Determine if the user interface should be displayed or not.
	if (input->KeyDown(DIK_F1))
	{
		m_displayUI = !m_displayUI;
	}

	return;
}

void ZoneClass::HandleMovementInput(InputClass * input, float frameTime)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	keyDown = input->KeyPressed(DIK_W);
	m_Position->MoveForward(keyDown);

	keyDown = input->KeyPressed(DIK_A);
	m_Position->MoveLeft(keyDown);

	keyDown = input->KeyPressed(DIK_S);
	m_Position->MoveBackward(keyDown);

	keyDown = input->KeyPressed(DIK_D);
	m_Position->MoveRight(keyDown);

	keyDown = input->KeyPressed(DIK_Q);
	m_Position->MoveUpward(keyDown);

	keyDown = input->KeyPressed(DIK_E);
	m_Position->MoveDownward(keyDown);

	keyDown = input->KeyPressed(DIK_LEFT);
	m_Position->TurnLeft(keyDown);

	keyDown = input->KeyPressed(DIK_RIGHT);
	m_Position->TurnRight(keyDown);

	keyDown = input->KeyPressed(DIK_UP);
	m_Position->LookUpward(keyDown);

	keyDown = input->KeyPressed(DIK_DOWN);
	m_Position->LookDownward(keyDown);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	return;
}

void ZoneClass::HandleGeneratorInput(ID3D11Device* device, InputClass* input)
{
	bool keyDown;

	if (input->KeyPressed(DIK_LCONTROL))
	{
		if (input->KeyPressed(DIK_1)) // handle seed change
		{
			keyDown = input->KeyDown(DIK_SPACE);
			if (keyDown)
			{
				m_Terrain->DisableSeed(device);
			}

			keyDown = input->KeyDown(DIK_NUMPADMINUS);
			if (keyDown)
			{
				m_Terrain->DecreaseSeed(device);
			}

			keyDown = input->KeyDown(DIK_NUMPADPLUS);
			if (keyDown)
			{
				m_Terrain->IncreaseSeed(device);
			}
		}
		else if (input->KeyPressed(DIK_2))
		{
			keyDown = input->KeyDown(DIK_SPACE);
			if (keyDown)
			{
				m_Terrain->DisableDisplacement(device);
			}

			keyDown = input->KeyDown(DIK_NUMPAD4);
			if (keyDown)
			{
				m_Terrain->DecreaseDisplacementX(device);
			}

			keyDown = input->KeyDown(DIK_NUMPAD2);
			if (keyDown)
			{
				m_Terrain->DecreaseDisplacementY(device);
			}

			keyDown = input->KeyDown(DIK_NUMPAD6);
			if (keyDown)
			{
				m_Terrain->IncreaseDisplacementX(device);
			}

			keyDown = input->KeyDown(DIK_NUMPAD8);
			if (keyDown)
			{
				m_Terrain->IncreaseDisplacementY(device);
			}
		}
		else if (input->KeyPressed(DIK_3))
		{
			keyDown = input->KeyDown(DIK_SPACE);
			if (keyDown)
			{
				m_Terrain->DisableFrequency(device);
			}

			keyDown = input->KeyDown(DIK_NUMPADMINUS);
			if (keyDown)
			{
				m_Terrain->DecreaseFrequency(device);
			}

			keyDown = input->KeyDown(DIK_NUMPADPLUS);
			if (keyDown)
			{
				m_Terrain->IncreaseFrequency(device);
			}
		}
		else if (input->KeyPressed(DIK_4))
		{
			keyDown = input->KeyDown(DIK_SPACE);
			if (keyDown)
			{
				m_Terrain->DisableTerraceSmoothness(device);
			}

			keyDown = input->KeyDown(DIK_NUMPADMINUS);
			if (keyDown)
			{
				m_Terrain->DecreaseTerraceSmoothness(device);
			}

			keyDown = input->KeyDown(DIK_NUMPADPLUS);
			if (keyDown)
			{
				m_Terrain->IncreaseTerraceSmoothness(device);
			}
		}
		else if (input->KeyPressed(DIK_5))
		{
			keyDown = input->KeyDown(DIK_SPACE);
			if (keyDown)
			{
				m_Terrain->DisableExponent(device);
			}

			keyDown = input->KeyDown(DIK_NUMPADMINUS);
			if (keyDown)
			{
				m_Terrain->DecreaseExponent(device);
			}

			keyDown = input->KeyDown(DIK_NUMPADPLUS);
			if (keyDown)
			{
				m_Terrain->IncreaseExponent(device);
			}
		}
	}

	return;
}

bool ZoneClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	bool result;

	// frustum culling
	bool renderModel;
	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color;

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);

	// Clear the buffers to begin the scene.
	Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Render the terrain grid using the color shader.
	m_Terrain->Render(Direct3D->GetDeviceContext());
	result = ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, TextureManager->GetTerrainTextures(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Terrain->GetBiomHeights());

	if (!result)
	{
		return false;
	}

	// Construct the frustum.
	m_Frustum->ConstructFrustum(SettingsHelperClass::SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Get the number of models that will be rendered.
	modelCount = m_ModelList->GetModelCount();

	// Initialize the count of models that have been rendered.
	renderCount = 0;

	Direct3D->GetWorldMatrix(worldMatrix);

	// Go through all the models and render them only if they can be seen by the camera view.
	for (index = 0; index < modelCount; index++)
	{
		// Get the position and color of the sphere model at this index.
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;

		// Check if the sphere model is in the view frustum.
		//renderModel = m_Frustum->CheckCube(positionX, positionY, positionZ, radius);
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model and check the next sphere.
		if (renderModel)
		{
			// Move the model to the location it should be rendered at.
			D3DXMatrixTranslation(&worldMatrix, positionX, positionY, positionZ);

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			m_Model->Render(Direct3D->GetDeviceContext());

			// Render the model using the light shader.
			ShaderManager->RenderLightShader(Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Model->GetTextureArray(), m_Light->GetDirection(), m_Light->GetAmbientColor(), color,
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			// Reset to the original world matrix.
			Direct3D->GetWorldMatrix(worldMatrix);

			// Since this model was rendered then increase the count for this frame.
			renderCount++;
		}
	}

	result = m_UI->SetRenderCount(renderCount, modelCount, Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Render the user interface.
	result = m_UI->Render(Direct3D, ShaderManager, worldMatrix, baseViewMatrix, orthoMatrix, m_displayUI);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	Direct3D->EndScene();

	return true;
}
