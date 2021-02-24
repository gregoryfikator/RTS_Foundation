#pragma once

#ifndef _UICLASS_H_
#define _UICLASS_H_

#include <iomanip>
#include <sstream>

#include "D3DClass.h"
#include "Models/BitmapClass.h"
#include "Models/TerrainClass.h"
#include "Models/TextClass.h"
#include "Shaders/ShaderManagerClass.h"

class UIClass
{
public:
	UIClass();
	UIClass(const UIClass&);
	~UIClass();

	bool Initialize(D3DClass*, HWND, int, int);
	void Shutdown();

	bool Frame(ID3D11DeviceContext*, int, int, int, int, int, int, float, float, float, float, float, float, TerrainClass*);
	bool Render(D3DClass*, ShaderManagerClass*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, bool);

	bool SetRenderCount(int, int, ID3D11DeviceContext*);
private:

	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetCameraPosition(float, float, float, ID3D11DeviceContext*);
	bool SetCameraRotation(float, float, float, ID3D11DeviceContext*);

	bool SetGeneratorParameters(TerrainClass*, ID3D11DeviceContext*);

private:
	FontClass* m_Font;

	TextClass* m_VideoStrings, * m_FpsString, * m_CpuString, * m_MousePositionString, * m_renderCountString, * m_PositionStrings, * m_RotationStrings;

	TextClass* m_GeneratorStrings;

	BitmapClass* m_CursorBitmap;

	int m_previousFps;
	int m_previousCpu;
	int m_previousMousePosition[2];
	int m_previousRenderCount;
	int m_previousRenderMaxCount;
	int m_previousPosition[3];
	int m_previousRotation[3];

	long m_previousGenerationTime;
	long m_previousGeneratorSeed;
	float m_previousGeneratorDisplacement[2];
	float m_previousGeneratorFrequency;
	float m_previousGeneratorTerraceSmoothness;
	float m_previousGeneratorExponent;

};

#endif