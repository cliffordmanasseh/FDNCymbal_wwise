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

#ifndef FDNCymbalSourceParams_H
#define FDNCymbalSourceParams_H

#include <AK/SoundEngine/Common/IAkPlugin.h>
#include <AK/Plugin/PluginServices/AkFXParameterChangeHandler.h>

// Add parameters IDs here, those IDs should map to the AudioEnginePropertyID
// attributes in the xml property definition.
static const AkPluginParamID PARAM_DURATION_ID = 0;
static const AkPluginParamID PARAM_SEED_ID = 1;
static const AkPluginParamID PARAM_RETRIGGERTIME_ID = 2;
static const AkPluginParamID PARAM_RETRIGGERSTICK_ID = 3;
static const AkPluginParamID PARAM_RETRIGGERTREMOLO_ID = 4;
//FDN
static const AkPluginParamID PARAM_FDN_ID = 5;
static const AkPluginParamID PARAM_FDNTIME_ID = 6;
static const AkPluginParamID PARAM_FDNFEEDBACK_ID = 7;
static const AkPluginParamID PARAM_FDNCASCADEMIX_ID = 8;
static const AkPluginParamID PARAM_ALLPASSMIX_ID = 9;
static const AkPluginParamID PARAM_ALLPASS1SATURATION_ID = 10;
static const AkPluginParamID PARAM_ALLPASS1TIME_ID = 11;
static const AkPluginParamID PARAM_ALLPASS1FEEDBACK_ID = 12;
static const AkPluginParamID PARAM_ALLPASS1HIGHPASSCUTOFF_ID = 13;
static const AkPluginParamID PARAM_ALLPASS2TIME_ID = 14;
static const AkPluginParamID PARAM_ALLPASS2FEEDBACK_ID = 15;
static const AkPluginParamID PARAM_ALLPASS2HIGHPASSCUTOFF_ID = 16;
//TERMOLO
static const AkPluginParamID PARAM_TREMOLOMIX_ID = 17;
static const AkPluginParamID PARAM_TREMOLODEPTH_ID = 18;
static const AkPluginParamID PARAM_TREMOLOFREQUENCY_ID = 19;
static const AkPluginParamID PARAM_TREMOLODELAYTIME_ID = 20;
static const AkPluginParamID PARAM_RANDOMTREMOLODEPTH_ID = 21;
static const AkPluginParamID PARAM_RANDOMTREMOLOFREQUENCY_ID = 22;
static const AkPluginParamID PARAM_RANDOMTREMOLODELAYTIME_ID = 23;
//STICK
static const AkPluginParamID PARAM_STICK_ID = 24;
static const AkPluginParamID PARAM_STICKDECAY_ID = 25;
static const AkPluginParamID PARAM_STICKTONEMIX_ID = 26;
static const AkPluginParamID PARAM_STICKPULSEMIX_ID = 27;
static const AkPluginParamID PARAM_STICKVELVETMIX_ID = 28;

static const AkPluginParamID PARAM_SMOOTHNESS_ID = 29;
static const AkPluginParamID PARAM_TRIGGER_ID = 30;
static const AkPluginParamID PARAM_PITCH_ID = 31;
static const AkUInt32 NUM_PARAMS = 32;

struct FDNCymbalRTPCParams
{
    AkReal32 fDuration;
    AkUInt32 fSeed;
    bool     fRetriggerTime;
    bool     fRetriggerStick;
    bool     fRetriggerTremolo;
    bool     fFDN;
    AkReal32 fFdnTime;
    AkReal32 fFdnFeedback;
    AkReal32 fFdnCascadeMix;
    AkReal32 fAllpassMix;
    bool     fAllpass1Saturation;
    AkReal32 fAllpass1Time;
    AkReal32 fAllpass1Feedback;
    AkReal32 fAllpass1HighpassCutoff;
    AkReal32 fAllpass2Time;
    AkReal32 fAllpass2Feedback;
    AkReal32 fAllpass2HighpassCutoff;

    AkReal32 fTremoloMix;
    AkReal32 fTremoloDepth;
    AkReal32 fTremoloFrequency;
    AkReal32 fTremoloDelayTime;
    AkReal32 fRandomTremoloDepth;
    AkReal32 fRandomTremoloFrequency;
    AkReal32 fRandomTremoloDelayTime;

    bool     fStick;
    AkReal32 fStickDecay;
    AkReal32 fStickToneMix;
    AkReal32 fStickPulseMix;
    AkReal32 fStickVelvetMix;

    AkReal32 fSmoothness;
    AkReal32 fTrigger;
    AkReal32 fPitch;


};

struct FDNCymbalNonRTPCParams
{
};

struct FDNCymbalSourceParams
    : public AK::IAkPluginParam
{
    FDNCymbalSourceParams();
    FDNCymbalSourceParams(const FDNCymbalSourceParams& in_rParams);

    ~FDNCymbalSourceParams();

    /// Create a duplicate of the parameter node instance in its current state.
    IAkPluginParam* Clone(AK::IAkPluginMemAlloc* in_pAllocator) override;

    /// Initialize the plug-in parameter node interface.
    /// Initializes the internal parameter structure to default values or with the provided parameter block if it is valid.
    AKRESULT Init(AK::IAkPluginMemAlloc* in_pAllocator, const void* in_pParamsBlock, AkUInt32 in_ulBlockSize) override;

    /// Called by the sound engine when a parameter node is terminated.
    AKRESULT Term(AK::IAkPluginMemAlloc* in_pAllocator) override;

    /// Set all plug-in parameters at once using a parameter block.
    AKRESULT SetParamsBlock(const void* in_pParamsBlock, AkUInt32 in_ulBlockSize) override;

    /// Update a single parameter at a time and perform the necessary actions on the parameter changes.
    AKRESULT SetParam(AkPluginParamID in_paramID, const void* in_pValue, AkUInt32 in_ulParamSize) override;

    AK::AkFXParameterChangeHandler<NUM_PARAMS> m_paramChangeHandler;

    FDNCymbalRTPCParams RTPC;
};

#endif // FDNCymbalSourceParams_H
