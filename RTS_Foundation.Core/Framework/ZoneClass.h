#pragma once

#ifndef _ZONECLASS_H_
#define _ZONECLASS_H_

#include "D3DClass.h"
#include "FrustumClass.h"
#include "InputClass.h"
#include "ModelListClass.h"
#include "PositionClass.h"
#include "TextureManagerClass.h"
#include "TimerClass.h"
#include "UIClass.h"
#include "Helpers/SettingsHelperClass.h"
#include "Models/CameraClass.h"
#include "Models/LightClass.h"
#include "Models/ModelClass.h"
#include "Models/TerrainClass.h"
#include "Shaders/ShaderManagerClass.h"

class ZoneClass
{
public:
	ZoneClass();
	ZoneClass(const ZoneClass&);
	~ZoneClass();

	bool Initialize(D3DClass*, HWND, int, int, TextureManagerClass*);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, ShaderManagerClass*, TextureManagerClass*, float, int, int);

private:
	void HandleInput(ID3D11Device*, InputClass*, float);
	void HandleGeneratorInput(ID3D11Device*, InputClass*);
	void HandleMovementInput(InputClass*, float);
	bool Render(D3DClass*, ShaderManagerClass*, TextureManagerClass*);

private:
	CameraClass* m_Camera;
	FrustumClass* m_Frustum;
	LightClass* m_Light;
	ModelClass* m_Model;
	ModelListClass* m_ModelList;
	PositionClass* m_Position;
	TerrainClass* m_Terrain;
	UIClass* m_UI;

	OBJLoader* m_objLoader;

	bool m_displayUI;
};

#endif