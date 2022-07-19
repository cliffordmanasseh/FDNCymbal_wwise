/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2021 Audiokinetic Inc.
*******************************************************************************/

#include "FDNCymbalPlugin.h"
#include "../SoundEnginePlugin/FDNCymbalSourceFactory.h"

const char* const szDuration= "Duration"
const char* const szSeed= "Seed"
const char* const szRetriggerTime="RetriggerTime"
const char* const szRetriggerStick ="RetriggerStick"
const char* const szRetriggerTremolo ="RetriggerTremolo"
const char* const szFDN ="FDN"
const char* const szFdnTime ="FdnTime"
const char* const szFdnFeedback ="FdnFeedback"
const char* const szFdnCascadeMix ="FdnCascadeMix"
const char* const szAllpassMix ="AllpassMix"
const char* const szAllpass1Saturation ="Allpass1Saturation"
const char* const szAllpass1Time ="Allpass1Time"
const char* const szAllpass1Feedback ="Allpass1Feedback"
const char* const szAllpass1HighpassCutoff ="Allpass1HighpassCutoff"
const char* const szAllpass2Time ="Allpass2Time"
const char* const szAllpass2Feedback ="Allpass2Feedback"
const char* const szAllpass2HighpassCutoff ="Allpass2HighpassCutoff"
const char* const szTremoloMix ="TremoloMix"
const char* const szTremoloDepth ="TremoloDepth"
const char* const szTremoloFrequency ="TremoloFrequency"
const char* const szTremoloDelayTime ="TremoloDelayTime"
const char* const szRandomTremoloDepth ="RandomTremoloDepth"
const char* const szRandomTremoloFrequency ="RandomTremoloFrequency"
const char* const szRandomTremoloDelayTime ="RandomTremoloDelayTime"
const char* const szStick ="Stick"
const char* const szStickDecay ="StickDecay"
const char* const szStickToneMix ="StickToneMix"
const char* const szStickPulseMix ="StickPulseMix"
const char* const szStickVelvetMix ="StickVelvetMix"
const char* const szSmoothness ="Smoothness"
const char* const szTrigger ="Trigger"
const char* const szPitch = "szPitch"

FDNCymbalPlugin::FDNCymbalPlugin()
{
}

FDNCymbalPlugin::~FDNCymbalPlugin()
{
}

bool FDNCymbalPlugin::GetBankParameters(const GUID & in_guidPlatform, AK::Wwise::Plugin::DataWriter& in_dataWriter) const
{
    // Write bank data here
    in_dataWriter.WriteReal32(m_propertySet.GetReal32(in_guidPlatform, "Duration"));

    return true;
}

DEFINE_AUDIOPLUGIN_CONTAINER(FDNCymbal);											// Create a PluginContainer structure that contains the info for our plugin
EXPORT_AUDIOPLUGIN_CONTAINER(FDNCymbal);											// This is a DLL, we want to have a standardized name
ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(                                             // Add our CLI class to the PluginContainer
    FDNCymbal,        // Name of the plug-in container for this shared library
    FDNCymbalPlugin,  // Authoring plug-in class to add to the plug-in container
    FDNCymbalSource   // Corresponding Sound Engine plug-in class
);
DEFINE_PLUGIN_REGISTER_HOOK

DEFINEDUMMYASSERTHOOK;							// Placeholder assert hook for Wwise plug-ins using AKASSERT (cassert used by default)
