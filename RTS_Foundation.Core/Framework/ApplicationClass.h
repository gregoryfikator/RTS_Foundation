#pragma once

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

#include "D3DClass.h"
#include "InputClass.h"
#include "TimerClass.h"
#include "FPSClass.h"
#include "CPUClass.h"
#include "TextureManagerClass.h"
#include "ZoneClass.h"
#include "Helpers/SettingsHelperClass.h"
#include "Shaders/ShaderManagerClass.h"

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	CPUClass* m_Cpu;
	D3DClass* m_Direct3D;
	FPSClass* m_Fps;
	InputClass* m_Input;
	ShaderManagerClass* m_ShaderManager;
	TextureManagerClass* m_TextureManager;
	TimerClass* m_Timer;
	ZoneClass* m_Zone;
};

#endif