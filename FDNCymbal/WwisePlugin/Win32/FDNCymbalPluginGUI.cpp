
#include "FDNCymbalPluginGUI.h"

AK_WWISE_PLUGIN_GUI_WINDOWS_BEGIN_POPULATE_TABLE(FDNProp)

AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(IDC_TRIGGER, szTrigger)
AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(IDC_FDN, szFDN)
AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(IDC_STICK, szStick)
AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(IDC_RETRIGSTICK, szRetriggerStick)
AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(IDC_RETRIGTIME, szRetriggerTime)
AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(IDC_RETRIGTREM, szRetriggerTime)
AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(IDC_SATURATE, szAllpass1Saturation)

AK_WWISE_PLUGIN_GUI_WINDOWS_END_POPULATE_TABLE()

bool FDNCymbalPluginGUI::GetDialog(AK::Wwise::Plugin::eDialog in_eDialog, UINT& out_uiDialogID, AK::Wwise::Plugin::PopulateTableItem*& out_pTable) const
{

	switch (in_eDialog)
	{
	case AK::Wwise::Plugin::SettingsDialog:
		out_uiDialogID = IDD_FDNC_BIG;
		out_pTable = FDNProp;
		return true;

	case AK::Wwise::Plugin::ContentsEditorDialog:
		out_uiDialogID = IDD_FDNC_SMALL;
		out_pTable = NULL;
		return true;
	}

	return false;

}

bool FDNCymbalPluginGUI::WindowProc(AK::Wwise::Plugin::eDialog in_eDialog, HWND in_hWnd, UINT in_message, WPARAM in_wParam, LPARAM in_lParam, LRESULT& out_lResult)
{
	switch (in_message)

	{

	case WM_INITDIALOG:
	{
		if (in_eDialog == AK::Wwise::Plugin::SettingsDialog)
			m_hwndPropView = in_hWnd;
	}
	break;

	case WM_DESTROY:
	{
		if (in_eDialog == AK::Wwise::Plugin::SettingsDialog)
			m_hwndPropView = NULL;
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(in_wParam))
		{

		case 1007:
		{
			if (m_propertySet.GetBool(m_host.GetCurrentPlatform(), szTrigger))
			{
				m_propertySet.SetValueBool(m_host.GetCurrentPlatform(), szTrigger, 0);
			}
			break;
		}

		case IDC_FDN:
		{

			if (IsDlgButtonChecked(in_hWnd, IDC_FDN) == BST_CHECKED)
			{
				enableFDN(true, in_hWnd);
			}
			else if (IsDlgButtonChecked(in_hWnd, IDC_FDN) == BST_UNCHECKED)
			{
				enableFDN(false, in_hWnd);;	// Disable controls
			}

			break;
		}

		case IDC_STICK:
		{
			if (IsDlgButtonChecked(in_hWnd, IDC_STICK) == BST_CHECKED)
			{
				enableStick(true, in_hWnd);
			}
			else if (IsDlgButtonChecked(in_hWnd, IDC_STICK) == BST_UNCHECKED)
			{
				enableStick(false, in_hWnd);;	// Disable controls
			}
			break;
		}
		}



	} // 


	}

	out_lResult = 0;
	return false;
}

void FDNCymbalPluginGUI::enableFDN(bool in, HWND in_hWnd)
{
	HWND hwndItem = GetDlgItem(in_hWnd, IDC_FDNTIME);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_FDNFB);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_FDNTIME);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_CASCADEMIX);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_ALLPASSMIX);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_AP1DELAYTIME);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_AP2DELAYTIME);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_AP1FB);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_AP2FB);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_AP1HPF);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_AP2HPF);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);
}

void FDNCymbalPluginGUI::enableStick(bool in, HWND in_hWnd)
{
	HWND hwndItem = GetDlgItem(in_hWnd, IDC_STICKDECAY);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_STICKTONEMIX);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_STICKPULSEMIX);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);

	 hwndItem = GetDlgItem(in_hWnd, IDC_STICKVELVETMIX);
	AKASSERT(hwndItem);
	::EnableWindow(hwndItem, in);
}


ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(
	FDNCymbal,            // Name of the plug-in container for this shared library
	FDNCymbalPluginGUI,   // Authoring plug-in class to add to the plug-in container
	FDNCymbalSource       // Corresponding Sound Engine plug-in class
);
