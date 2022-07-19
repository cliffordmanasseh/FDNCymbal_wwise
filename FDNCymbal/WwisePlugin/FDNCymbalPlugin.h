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

#pragma once

#include <AK/Wwise/Plugin.h>

extern const char* const szDuration;
extern const char* const szSeed;
extern const char* const szRetriggerTime;
extern const char* const szRetriggerStick;
extern const char* const szRetriggerTremolo;
extern const char* const szFDN;
extern const char* const szFdnTime;
extern const char* const szFdnFeedback;
extern const char* const szFdnCascadeMix;
extern const char* const szAllpassMix;
extern const char* const szAllpass1Saturation;
extern const char* const szAllpass1Time;
extern const char* const szAllpass1Feedback;
extern const char* const szAllpass1HighpassCutoff;
extern const char* const szAllpass2Time;
extern const char* const szAllpass2Feedback;
extern const char* const szAllpass2HighpassCutoff;
extern const char* const szTremoloMix;
extern const char* const szTremoloDepth;
extern const char* const szTremoloFrequency;
extern const char* const szTremoloDelayTime;
extern const char* const szRandomTremoloDepth;
extern const char* const szRandomTremoloFrequency;
extern const char* const szRandomTremoloDelayTime;
extern const char* const szStick;
extern const char* const szStickDecay;
extern const char* const szStickToneMix;
extern const char* const szStickPulseMix;
extern const char* const szStickVelvetMix;
extern const char* const szSmoothness;
extern const char* const szTrigger;
extern const char* const szPitch;
/// See https://www.audiokinetic.com/library/edge/?source=SDK&id=plugin__dll.html
/// for the documentation about Authoring plug-ins
class FDNCymbalPlugin final
    : public AK::Wwise::Plugin::AudioPlugin
    , public AK::Wwise::Plugin::Source
    , public AK::Wwise::Plugin::RequestHost
{
public:
    FDNCymbalPlugin();
    ~FDNCymbalPlugin();

    /// This function is called by Wwise to obtain parameters that will be written to a bank.
    /// Because these can be changed at run-time, the parameter block should stay relatively small.
    // Larger data should be put in the Data Block.
    bool GetBankParameters(const GUID & in_guidPlatform, AK::Wwise::Plugin::DataWriter& in_dataWriter) const override;
    virtual bool GetSourceDuration(double& out_dblMinDuration, double& out_dblMaxDuration) const override;

};

DECLARE_AUDIOPLUGIN_CONTAINER(FDNCymbal);	// Exposes our PluginContainer structure that contains the info for our plugin
