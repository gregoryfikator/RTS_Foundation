#pragma once

#ifndef _SETTINGSHELPERCLASS_H_
#define _SETTINGSHELPERCLASS_H_

class SettingsHelperClass
{
public:
	static bool FULL_SCREEN;
	static bool VSYNC_ENABLED;
	static float SCREEN_DEPTH;
	static float SCREEN_NEAR;

private:
	SettingsHelperClass() {}
};

#endif

