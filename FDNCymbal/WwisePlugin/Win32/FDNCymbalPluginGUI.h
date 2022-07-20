#pragma once

#include "../FDNCymbalPlugin.h"
#include "../resource.h"

class FDNCymbalPluginGUI final
	: public AK::Wwise::Plugin::RequestHost
	, public AK::Wwise::Plugin::PluginMFCWindows<>
	, public AK::Wwise::Plugin::GUIWindows
	, public AK::Wwise::Plugin::RequestPropertySet
{
public:
	virtual bool GetDialog(AK::Wwise::Plugin::eDialog in_eDialog, UINT& out_uiDialogID, AK::Wwise::Plugin::PopulateTableItem*& out_pTable) const override;
	virtual bool WindowProc(AK::Wwise::Plugin::eDialog in_eDialog, HWND in_hWnd, UINT in_message, WPARAM in_wParam, LPARAM in_lParam, LRESULT& out_lResult) override;
	void enableFDN(bool in, HWND in_hWnd);
	void enableStick(bool in, HWND in_hWnd);

	FDNCymbalPluginGUI() {};
	~FDNCymbalPluginGUI() {};

private:
	HWND m_hwndPropView = nullptr;
	HWND m_hwndObjPane = nullptr;

};
