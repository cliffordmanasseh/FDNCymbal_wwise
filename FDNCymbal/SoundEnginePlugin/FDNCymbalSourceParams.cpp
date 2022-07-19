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

#include "FDNCymbalSourceParams.h"

#include <AK/Tools/Common/AkBankReadHelpers.h>

FDNCymbalSourceParams::FDNCymbalSourceParams()
{
}

FDNCymbalSourceParams::~FDNCymbalSourceParams()
{
}

FDNCymbalSourceParams::FDNCymbalSourceParams(const FDNCymbalSourceParams& in_rParams)
{
    RTPC = in_rParams.RTPC;
    m_paramChangeHandler.SetAllParamChanges();
}

AK::IAkPluginParam* FDNCymbalSourceParams::Clone(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, FDNCymbalSourceParams(*this));
}

AKRESULT FDNCymbalSourceParams::Init(AK::IAkPluginMemAlloc* in_pAllocator, const void* in_pParamsBlock, AkUInt32 in_ulBlockSize)
{
    if (in_ulBlockSize == 0)
    {
        // Initialize default parameters here
            RTPC.fDuration              = 0.0f;
            RTPC.fSeed                  = 6583421;
            RTPC.fRetriggerTime         = true;
            RTPC.fRetriggerStick        = true;
            RTPC.fRetriggerTremolo      = true;
            RTPC.fFDN                   = true;
            RTPC.fFdnTime               = 0.2f;
            RTPC.fFdnFeedback           = 0.5f;
            RTPC.fFdnCascadeMix         = 0.5f;
            RTPC.fAllpassMix            = 0.75f;
            RTPC.fAllpass1Saturation    = true;
            RTPC.fAllpass1Time          = 0.5f;
            RTPC.fAllpass1Feedback      = 0.75f;
            RTPC.fAllpass1HighpassCutoff= 0.5f;
            RTPC.fAllpass2Time          = 0.5f;
            RTPC.fAllpass2Feedback      = 0.5f;
            RTPC.fAllpass2HighpassCutoff= 0.5f;
            RTPC.fTremoloMix            = 0.2f;
            RTPC.fTremoloDepth          = 0.5f;
            RTPC.fTremoloFrequency      = 0.5f;
            RTPC.fTremoloDelayTime      = 0.25f;
            RTPC.fRandomTremoloDepth    = 0.25f;
            RTPC.fRandomTremoloFrequency= 0.35f;
            RTPC.fRandomTremoloDelayTime= 0.35f;
            RTPC.fStick                 = true;
            RTPC.fStickDecay            = 0.5f;
            RTPC.fStickToneMix          = 0.5f;
            RTPC.fStickPulseMix         = 1.0f;
            RTPC.fStickVelvetMix        = 1.0f;
            RTPC.fSmoothness            = 0.1f;
            RTPC.fTrigger               = false;
        m_paramChangeHandler.SetAllParamChanges();
        return AK_Success;
    }

    return SetParamsBlock(in_pParamsBlock, in_ulBlockSize);
}

AKRESULT FDNCymbalSourceParams::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT FDNCymbalSourceParams::SetParamsBlock(const void* in_pParamsBlock, AkUInt32 in_ulBlockSize)
{
    AKRESULT eResult = AK_Success;
    AkUInt8* pParamsBlock = (AkUInt8*)in_pParamsBlock;

    // Read bank data here
    RTPC.fDuration = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fSeed = READBANKDATA(AkUInt32, pParamsBlock, in_ulBlockSize);
    RTPC.fRetriggerTime = READBANKDATA(bool, pParamsBlock, in_ulBlockSize);
    RTPC.fRetriggerStick = READBANKDATA(bool, pParamsBlock, in_ulBlockSize);
    RTPC.fRetriggerTremolo = READBANKDATA(bool, pParamsBlock, in_ulBlockSize);
    RTPC.fFDN = READBANKDATA(bool, pParamsBlock, in_ulBlockSize);
    RTPC.fFdnTime = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fFdnFeedback = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fFdnCascadeMix = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fAllpassMix = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fAllpass1Saturation = READBANKDATA(bool, pParamsBlock, in_ulBlockSize);
    RTPC.fAllpass1Time = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fAllpass1Feedback = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fAllpass1HighpassCutoff = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fAllpass2Time = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fAllpass2Feedback = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fAllpass2HighpassCutoff = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fTremoloMix = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fTremoloDepth = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fTremoloFrequency = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fTremoloDelayTime = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fRandomTremoloDepth = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fRandomTremoloFrequency = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fRandomTremoloDelayTime = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fStick = READBANKDATA(bool, pParamsBlock, in_ulBlockSize);
    RTPC.fStickDecay = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fStickToneMix = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fStickPulseMix = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fStickVelvetMix = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fSmoothness = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);
    RTPC.fPitch = READBANKDATA(AkReal32, pParamsBlock, in_ulBlockSize);

    CHECKBANKDATASIZE(in_ulBlockSize, eResult);
    m_paramChangeHandler.SetAllParamChanges();

    return eResult;
}

AKRESULT FDNCymbalSourceParams::SetParam(AkPluginParamID in_paramID, const void* in_pValue, AkUInt32 in_ulParamSize)
{
    AKRESULT eResult = AK_Success;

    // Handle parameter change here
    switch (in_paramID)
    {
    case PARAM_DURATION_ID:
        RTPC.fDuration = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_DURATION_ID);
        break;
    case PARAM_SEED_ID:
        RTPC.fSeed = (AkUInt32)*((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_SEED_ID);
        break;
    case PARAM_RETRIGGERTIME_ID:
        RTPC.fRetriggerTime = (bool)*((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_RETRIGGERTIME_ID);
        break;
    case PARAM_RETRIGGERSTICK_ID:
        RTPC.fRetriggerStick = (bool)*((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_RETRIGGERSTICK_ID);
        break;
    case PARAM_RETRIGGERTREMOLO_ID:
        RTPC.fRetriggerTremolo = (bool)*((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_RETRIGGERTREMOLO_ID);
        break;
    case PARAM_FDN_ID:
        RTPC.fFDN = (bool)*((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_FDN_ID);
        break;

    case PARAM_FDNTIME_ID:
        RTPC.fFdnTime = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_FDNTIME_ID);
        break;
    case PARAM_FDNFEEDBACK_ID:
        RTPC.fFdnFeedback = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_FDNFEEDBACK_ID);
        break;
    case PARAM_FDNCASCADEMIX_ID:
        RTPC.fFdnCascadeMix = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_FDNCASCADEMIX_ID);
        break;
    case PARAM_ALLPASSMIX_ID:
        RTPC.fAllpassMix = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_ALLPASSMIX_ID);
        break;
    case PARAM_ALLPASS1SATURATION_ID:
        RTPC.fAllpass1Saturation = (bool)*((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_ALLPASS1SATURATION_ID);
        break;
    case PARAM_ALLPASS1TIME_ID:
        RTPC.fAllpass1Time = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_ALLPASS1TIME_ID);
        break;
    case PARAM_ALLPASS1FEEDBACK_ID:
        RTPC.fAllpass1Feedback = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_ALLPASS1FEEDBACK_ID);
        break;
    case PARAM_ALLPASS1HIGHPASSCUTOFF_ID:
        RTPC.fAllpass1HighpassCutoff = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_ALLPASS1HIGHPASSCUTOFF_ID);
        break;
    case PARAM_ALLPASS2TIME_ID:
        RTPC.fAllpass2Time = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_ALLPASS2TIME_ID);
        break;
    case PARAM_ALLPASS2FEEDBACK_ID:
        RTPC.fAllpass2Feedback = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_ALLPASS2FEEDBACK_ID);
        break;
    case PARAM_ALLPASS2HIGHPASSCUTOFF_ID:
        RTPC.fAllpass2HighpassCutoff = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_ALLPASS2HIGHPASSCUTOFF_ID);
        break;

    case PARAM_TREMOLOMIX_ID:
        RTPC.fTremoloMix = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_TREMOLOMIX_ID);
        break;
    case PARAM_TREMOLODEPTH_ID:
        RTPC.fTremoloDepth = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_TREMOLODEPTH_ID);
        break;
    case PARAM_TREMOLOFREQUENCY_ID:
        RTPC.fTremoloFrequency = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_TREMOLOFREQUENCY_ID);
        break;
    case PARAM_TREMOLODELAYTIME_ID:
        RTPC.fTremoloDelayTime = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_TREMOLODELAYTIME_ID);
        break;
    case PARAM_RANDOMTREMOLODEPTH_ID:
        RTPC.fRandomTremoloDepth = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_RANDOMTREMOLODEPTH_ID);
        break;
    case PARAM_RANDOMTREMOLOFREQUENCY_ID:
        RTPC.fRandomTremoloFrequency = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_RANDOMTREMOLOFREQUENCY_ID);
        break;
    case PARAM_RANDOMTREMOLODELAYTIME_ID:
        RTPC.fRandomTremoloDelayTime = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_RANDOMTREMOLODELAYTIME_ID);
        break;

    case PARAM_STICK_ID:
        RTPC.fStick = (bool)*((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_STICK_ID);
        break;
    case PARAM_STICKDECAY_ID:
        RTPC.fStickDecay = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_STICKDECAY_ID);
        break;
    case PARAM_STICKTONEMIX_ID:
        RTPC.fStickToneMix = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_STICKTONEMIX_ID);
        break;
    case PARAM_STICKPULSEMIX_ID:
        RTPC.fStickPulseMix = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_STICKPULSEMIX_ID);
        break;
    case PARAM_STICKVELVETMIX_ID:
        RTPC.fStickVelvetMix = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_STICKVELVETMIX_ID);
        break;

    case PARAM_SMOOTHNESS_ID:
        RTPC.fSmoothness = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_SMOOTHNESS_ID);
        break;
    case PARAM_TRIGGER_ID:
        RTPC.fTrigger = (bool)*((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_TRIGGER_ID);
        break;
    case PARAM_PITCH_ID:
        RTPC.fPitch = *((AkReal32*)in_pValue);
        m_paramChangeHandler.SetParamChange(PARAM_PITCH_ID);
        break;

    default:
        eResult = AK_InvalidParameter;
        break;
    }

    return eResult;
}
