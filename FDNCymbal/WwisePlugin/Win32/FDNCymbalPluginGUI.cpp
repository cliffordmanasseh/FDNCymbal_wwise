
#include "FDNCymbalPluginGUI.h"

FDNCymbalPluginGUI::FDNCymbalPluginGUI()
{
}

ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(
    FDNCymbal,            // Name of the plug-in container for this shared library
    FDNCymbalPluginGUI,   // Authoring plug-in class to add to the plug-in container
    FDNCymbalSource       // Corresponding Sound Engine plug-in class
);
