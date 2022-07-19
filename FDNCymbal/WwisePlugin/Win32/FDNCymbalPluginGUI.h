#pragma once

#include "../FDNCymbalPlugin.h"

class FDNCymbalPluginGUI final
	: public AK::Wwise::Plugin::PluginMFCWindows<>
	, public AK::Wwise::Plugin::GUIWindows
{
public:
	FDNCymbalPluginGUI();

};
